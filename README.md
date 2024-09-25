# AviUtl プラグイン - スーパーカットプラグイン
スーパーカットプラグイン(m_split_timeline.auf)は、拡張編集ウィンドウに配置されてる複数のオブジェクトを、再生位置で分割するAviUtlのプラグインです。(言葉で説明するのﾑｽﾞｶｼｲ)

[ダウンロードはこちら](https://github.com/yuratexi/aviutl_supercut/releases/latest)

詳しくは紹介動画をご覧ください。

紹介動画▷準備中

このプラグインが人生で初めて作ったプラグインなので多分色々拙いですがご了承ください。




# 動作要件
* (必須)AviUtl version1.10
* (必須)拡張編集 version0.92 (それ以外のバージョンには対応してません)
* (多分いる)Microsoft Visual C++ 2015-2022 再頒布可能パッケージ (x86)
  * [ここから最新版がインストールできます](https://learn.microsoft.com/ja-jp/cpp/windows/latest-supported-vc-redist?view=msvc-170)
* (多分いる)patch.aul
  * [導入してない人はこのサイトを参考に導入しましょう](https://scrapbox.io/nazosauna/patch.aul)

# 導入方法
1. [Releaseページ](https://github.com/yuratexi/aviutl_supercut/releases/latest)のAssets欄からm_split_timeline.zipをダウンロード
2. zipファイルを展開して出てきた"m_split_timeline.auf"をaviutl.exeがあるフォルダ(またはその直下のpluginsフォルダ)に配置

# 仕様
紹介動画のほうで詳しく解説してます。ですがこちらにも一応ざっくり書きます。

* ショートカットに以下の11項目を追加します
  * 分割ALL
  * 分割カスタム1
  * 分割カスタム2
  * 分割カスタム3
  * 分割カスタム4
  * 分割カスタム5
  * 分割カスタム6
  * 分割カスタム7
  * 分割カスタム8
  * 分割カスタム9
  * 分割カスタム10

基本的にはショートカットキーを割り当てて使用するものになると思います(編集→スーパーカット byゆらてぃー から使うこともできますが...)

* 分割ALL
  * 再生位置にあるオブジェクト全てを再生位置で分割します

* 分割カスタム1~10
  * 再生位置にあるオブジェクトのうち、予め指定しておいた単一・複数レイヤーにあるオブジェクト全てを再生位置で分割します
  * 分割カスタム1~10の設定方法は以下「設定方法」参照

# 設定方法
初回起動時、aviutl.exeと同じ階層に m_supercut_config.ini m_supercut_config_inst.txtの2つのファイルが生成されます。
ファイルの内容としては
* m_supercut_config.ini:設定ファイル
* m_supercut_config_inst.txt:設定方法を記したファイル

となっています。
編集すべきは、m_supercut_config.ini になります。
これをエクスプローラーから直接編集してもいいですし、aviutlの設定→スーパーカット byゆらてぃー の設定をクリック
出てきたウィンドウの「configファイルを開く」というボタンを押せば、設定ファイル(m_supercut_config.ini)が開きます。

設定方法の詳細として、以下にm_supercut_config_inst.txtファイルの中身を書いておきます  

  
>注意:configファイルに無駄な文字や改行を入れないでください。入れるとプラグインが正常に動作しません。
>
>
>-スーパーカット,分割カスタム1～10の設定方法-
>
>configファイルの" = "の後に、分割したいレイヤー番号を全て コンマ区切り で書きます
>レイヤー番号は昇順(小さい数から大きい数)になるように書いてください
>
>
>例えば、custom1を以下のように書き、aviutlメニューバーの編集→スーパーカット→分割カスタム1をクリックすると(もしくは対応するショートカットキーを押すと)
>
>custom1=1,2,3
>
>レイヤー1,2,3にあるオブジェクトが再生位置で分割されます
>
>
>設定できるのはcustom1～custom10までです、custom11= と書いても分割カスタム11が使えるといったことはありません。(今後対応する可能性はありますが...)

# 既知の不具合
・分割ALL,分割カスタムが時々作動しないことがある(原因不明)

# クレジット
* aviutl [https://spring-fragrance.mints.ne.jp/aviutl/](https://spring-fragrance.mints.ne.jp/aviutl/)
* aviutl_exedit_sdk [https://github.com/ePi5131/aviutl_exedit_sdk](https://github.com/ePi5131/aviutl_exedit_sdk)
* 切り取りプラグイン [https://github.com/nazonoSAUNA/tl_Item_cut?tab=readme-ov-file](https://github.com/nazonoSAUNA/tl_Item_cut?tab=readme-ov-file)

# ライセンスについて
aviutl_exedit_sdkの配布元がライセンスを1条項BSDとしているため、1条項BSDのライセンスをそのまま写しています。
ですが、その下にMITライセンスを追加しています。なので基本的にはMITライセンスとして扱ってもらって問題ないはずです。

# 作成者情報
* 作成者:ゆらてぃー
  * 普段はグループでマイクラ実況とか、まれにゆっくり実況とかやってます
* X(ゆらてぃー):[https://x.com/yuratexi5116](https://x.com/yuratexi5116)
* X(ゆらてぃー研究部(なにかあればこちらのDMまで...)):[https://x.com/researcheryura](https://x.com/researcheryura)
* Youtube(ゆらてぃー(ゆっくり実況)):[https://www.youtube.com/@ゆらてぃー](https://www.youtube.com/@%E3%82%86%E3%82%89%E3%81%A6%E3%81%83%E3%83%BC)
* Youtube(ねころ部(グループ実況)):[https://www.youtube.com/@ねころ部games](https://www.youtube.com/@%E3%81%AD%E3%81%93%E3%82%8D%E9%83%A8games)

# 免責事項
この作成物および同梱物を使用したことによって生じたすべての障害・損害・不具合等に関しては、作成者と作成者の関係者および作成者の所属するいかなる団体・組織とも、一切の責任を負いません。各自の責任においてご使用ください。  
本プラグインは、なんらの予告もなく変更、配布の中止をすることがあります。作成者と作成者の関係者および作成者の所属するいかなる団体・組織とも、これらを原因として発生した損失や損害について一切の責任を負いません。
