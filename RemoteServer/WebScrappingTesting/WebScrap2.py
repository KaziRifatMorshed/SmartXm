from selenium import webdriver
from selenium.webdriver.chrome.options import Options
import time

url = "https://cseku-smartxm.free.nf/pages/ecsv.php?keyChabi=iAmSuperSecret123&seed=20"
chrome_options = Options()
chrome_options.add_argument("--headless")
chrome_options.add_argument("--disable-gpu")
prefs = {
    "profile.managed_default_content_settings.images": 2,
    "profile.managed_default_content_settings.stylesheets": 2,
    "profile.managed_default_content_settings.fonts": 2,
    "profile.managed_default_content_settings.plugins": 2
}
chrome_options.add_experimental_option("prefs", prefs)

driver = webdriver.Chrome(options=chrome_options)
driver.get(url)

# Wait for JS challenges if necessary (increase if needed)
time.sleep(3)

result = driver.page_source
# Clean up the result
result = result.replace("<br>", "\n")
result = result.replace("&amp;", "&")
result = result.replace('<html><head></head><body>', "")
result = result.replace('</body></html>', "")
print(result.strip())
driver.quit()

with open("./remoteData.csv", "w", encoding="utf-8") as ff:
    ff.write(result)

# pyinstaller --onefile main.py
