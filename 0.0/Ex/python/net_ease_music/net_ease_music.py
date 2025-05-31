# 访问 https://www.jetbrains.com/help/pycharm/ 获取 PyCharm 帮助

url = 'https://music.163.com/discover/toplist?id=3778678'
headers = {
    'user-agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/103.0.5060.'
                  '53 Safari/537.36 Edg/103.0.1264.37'}
response = requests.get(url=url, headers=headers)


#print(response.text)


html_data = re.findall('<li><a href="\/song\?id=(\d+)">(.*?)<\/a>', response.text)
for num_id, title in html_data:
    music_url = f'http://music.com/song/media/outer/url?id={num_id}.mp3'
    music_content = requests.get(music_url, headers).content
    with open('music\\' + title + '.mp3', mode='wb') as f:
        f.write(music_content)
    print(num_id, title)


