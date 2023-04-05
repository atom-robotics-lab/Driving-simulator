import sys
import requests
from bs4 import BeautifulSoup, SoupStrainer

home_url = 'https://parivahan.gov.in/rcdlstatus/'
post_url = 'https://parivahan.gov.in/rcdlstatus/vahan/rcDlHome.xhtml'
# Everything before the last four digits: MH02CL
first = sys.argv[1]
# The last four digits: 0555
second = sys.argv[2]

r = requests.get(url=home_url)
cookies = r.cookies
soup = BeautifulSoup(r.text, 'html.parser')
viewstate = soup.select('input[name="javax.faces.ViewState"]')[0]['value']

data = {
    'javax.faces.partial.ajax':'true',
    'javax.faces.source': 'form_rcdl:j_idt32',
    'javax.faces.partial.execute':'@all',
    'javax.faces.partial.render': 'form_rcdl:pnl_show form_rcdl:pg_show form_rcdl:rcdl_pnl',
    'form_rcdl:j_idt32':'form_rcdl:j_idt32',
    'form_rcdl':'form_rcdl',
    'form_rcdl:tf_reg_no1': first,
    'form_rcdl:tf_reg_no2': second,
    'javax.faces.ViewState': viewstate,
}

r = requests.post(url=post_url, data=data, cookies=cookies)
soup = BeautifulSoup(r.text, 'html.parser')
table = SoupStrainer('tr')
soup = BeautifulSoup(soup.get_text(), 'html.parser', parse_only=table)
print(soup.get_text())