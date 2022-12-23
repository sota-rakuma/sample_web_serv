# sample_web_serv

## nginxでのテスト方法

- nginxコンテナの起動
```sh
docker-compose up -d --build
```
- dockerコンテナの終了
```sh
docker-compose down
```
- コンテナの起動状況の確認
```sh
docker ps -a
```
- nginxコンテナの中に入る
```sh
docker exec -it ngnix /bin/bash
```

## TELNETの使い方
```bash
telnet 127.0.0.1 80
```
example
```
GET / HTTP/1.1
Host: localhost

```

## curlでCGIを呼び出す方法
**a.conf のtest.cgiと書かれてる箇所を適当なファイルに書き換えて、コンテナを実行したのちにアクセスすると、任意のcgiスクリプトをnginxで呼び出すせる**
```bash
curl localhost:80/www/
```
- header の内容だけ欲しい場合
```bash
curl -I localhost:80/www/
```
- HTTPメソッドの指定
examples
```bash
curl http://localhost:80/www/ -XPOST -d 'title=TEST id=111'
```
-Xでメソッドの指定ができ、-d でデータの送信ができる

