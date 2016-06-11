= ボリュームを変更 Ver.1.12
== どんなの？
ボリュームを変更、ミュートのON/OFFを行う、
((<TTBase|URL:http://web.kyoto-inet.or.jp/people/koyabu/TTBase.html>))用Pluginです。

== 割り当て可能なコマンド一覧。
: 「ミュート」
実行するたびに、ミュートのON/OFFを行います。

: 「音量をn上げる」「音量をn下げる」
音量をnパーセント分上下させます。

: 「音量をnにセット」
音量をnパーセントに設定します。

== 設定。
iniファイルを使用することで、設定を変更することが出来ます。
設定可能な内容は下記の通りです。
尚、コマンドを動的に作成している関係か、設定を変更したらTTBaseを再起動する必要があるようです。

=== Commandセクション
: UpDown0～9
「音量をn上げる」「音量をn下げる」で上下させるパーセント数を指定します。
0～9のそれぞれに別のパーセントを指定することで、最大10種類のコマンドを作成できます。

: Set0～9
「音量をnにセット」で設定するパーセントを指定します。
0～9のそれぞれに別のパーセントを指定することで、最大10種類のコマンドを作成できます。

=== Settingセクション
: VolComponentType
ボリューム調節デバイスの設定をします。
設定できる内容は((<TClock2chのヘルプ|URL:http://homepage3.nifty.com/TClock2ch/help2ch/option.html>))によると

  MIXERLINE_COMPONENTTYPE_DST_UNDEFINED    0
  MIXERLINE_COMPONENTTYPE_DST_DIGITAL      1
  MIXERLINE_COMPONENTTYPE_DST_LINE         2
  MIXERLINE_COMPONENTTYPE_DST_MONITOR      3
  MIXERLINE_COMPONENTTYPE_DST_SPEAKERS     4  //メインボリューム
  MIXERLINE_COMPONENTTYPE_DST_HEADPHONES   5
  MIXERLINE_COMPONENTTYPE_DST_TELEPHONE    6
  MIXERLINE_COMPONENTTYPE_DST_WAVEIN       7
  MIXERLINE_COMPONENTTYPE_DST_VOICEIN      8
  MIXERLINE_COMPONENTTYPE_SRC_UNDEFINED    9
  MIXERLINE_COMPONENTTYPE_SRC_DIGITAL     10
  MIXERLINE_COMPONENTTYPE_SRC_LINE        11
  MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE  12
  MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER 13
  MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC 14
  MIXERLINE_COMPONENTTYPE_SRC_TELEPHONE   15
  MIXERLINE_COMPONENTTYPE_SRC_PCSPEAKER   16
  MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT     17  //WAVE音量らしい
  MIXERLINE_COMPONENTTYPE_SRC_AUXILIARY   18
  MIXERLINE_COMPONENTTYPE_SRC_ANALOG      19

ということらしいです。私はよくわかってません(T-T)
デフォルトでは4が設定されます。

== 動作環境。
ボリュームを変更、はTTBase用Pluginです。
TTBaseが無いと動かせません。
((<K2 Software's page|URL:http://web.kyoto-inet.or.jp/people/koyabu/>))より別途入手してください。

== インストール。
解凍して、TTBaseが存在するフォルダ以下に置いてください。
その後TTBaseを再起動すれば、完了です。
また、再起動しないでも「TTBaseの設定」ダイアログを出した後「OK」を押しても、プラグインを再ロードしてくれるようです。

== アンインストール。
解凍したファイル全てを消してくれればおっけぇ。

== 著作権やら免責事項やら。
本配布物(Exeやらソースやらね)は私ことももたろが作成しました。
ので、一応、著作権は私が持ってることになります。
が、あんまりうるさいことは言いませんってゆーか言いたくありませんので、
商用・非商用かかわらず自由に利用してしまってください。

ソースが配布されている場合、そのソースを利用して作成されたバイナリ及びソースを配布する際に、
私に断りを入れる必要はありませんし、そのことを明記する必要もありません。
・・・とはいえ、一言教えてくれたりすると私が小躍りして喜びますし、
まったく同じものを著作権表示だけ変えて公開されたりすると不愉快な気分になります。
まぁ、そこらへんは利用者の良心にお任せします。

あ、そうそ。これを書いとかないと。
本配布物を利用する為の対価は一切頂きませんが、
そのかわり、利用したことによって起こった如何なる問題に関して
ももたろは一切その責任を負いません。ってゆーか負えません。

まぁ、簡単に言うと
「タダで配布するんで、勝手に使ってちょーだいな。その代わり何があっても知らないよ」
ってことです。

・・・といつもはココまでなのですが、mixer.cppは私が作ったものじゃないので、
上記内容にあてはまりません。別途mixer.cppを見てくださいな。

== スペシャルサンクス。
* さまざまな可能性を秘めたTTBaseを開発した、((<K2|URL:http://web.kyoto-inet.or.jp/people/koyabu/>))さま。
* コアのプログラムの参考(・・・っつーかパクリ)にさせていただいた、TClock2chの改造者、◆TDQ3TClockさま。
* ◆TDQ3TClock神も参考にした、((<窓プログラミング|URL:http://hp.vector.co.jp/authors/VA016117/>))さんち。
* 私がプログラムを作るたびに、検証・助言等をしてくれるOratorさま、藤代千尋さま。
* そして、このソフトを使ってくれるであろうみなみなさま。

みなさまの協力がなければ、このソフトはなかったのです。
ほんとに、ありがとさまでしたm(_ _)m

== れんらくさきだよ
* ((<URL:mailto:info@momo-lab.net>))
* ((<ももらぼっ!|URL:http://www.momo-lab.net>))

== いままでのれきし
=== Ver.1.12 (2003/02/08)
* 添付するiniファイルは拡張子を.sampleとするようにした。
* ReadMeファイル(このファイルね)の名称を変更した。

=== Ver.1.11 (2003/01/28)
* 時計に送るメッセージを、TCLOCKM_REFRESHCLOCKからTCLOCKM_VOLCHANGEに変更した。
* 上記修正に伴い、リフレッシュ時のちらつきを抑えられるようになった為、IsRefreshClockオプションを廃止した。

=== Ver.1.10 (2003/01/28)
* tcredraw.exeを参考に ボリュームを変更後TClockが入っていれば時計を再描画するようにした。

=== Ver.1.00 (2003/01/16)
* TClock2chのソース等を参考に作ってみる。

== こんごのよてー・・・はみてー
* 今のところなし。

Copyright(c) 2000-2003 ももたろ
