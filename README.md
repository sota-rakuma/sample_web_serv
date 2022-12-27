# sample_web_serv

## nginxでのテスト方法

- expディレクトリに移動
```sh
cd exp
```

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
docker exec -it nginx /bin/bash
```

**以下の項目はコンテナが起動した状態であれば、使用可能です。**

## TELNETの使い方
```bash
telnet 127.0.0.1 8080
```
example
```
GET / HTTP/1.1
Host: localhost

```

## curlでCGIを呼び出す方法
**a.conf のtest.cgiと書かれてる箇所を適当なファイルに書き換えて、コンテナを実行したのちにアクセスすると、任意のcgiスクリプトをnginxで呼び出せる**
```bash
curl localhost:8080/www/
```
- header の内容だけ欲しい場合
```bash
curl -I localhost:8080/www/
```
- HTTPメソッドの指定
examples
```bash
curl http://localhost:8080/www/ -XPOST -d 'title=TEST id=111'
```
-Xでメソッドの指定ができ、-d でデータの送信ができる

- ヘッダーをつける
```bash
curl -H "Host: test.com" 
```
## ブラウザ上でリクエスト/レスポンスを確認する方法
- Google Chrome
1. デベロッパーツール(開発者ツールを開く)
1. ネットワークと書かれた箇所をクリック
1. 真ん中あたりに出てくる名前の欄から見たいファイルをクリック
**コンテナの起動後にブラウザの検索バーで http://localhost:8080/src/ と打つとautoindexがonになってるので色々実験できると思います。**
