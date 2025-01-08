# 目標
何でもやってくれる至れり尽くせりなエンジンにする。
軽量化を怠らず速度を出す。そのうえで安全第一。

# TODO
- Engine部をまとめる
	- Singletonがあることを前提にしてエンジンの解放をまとめられる方法がないか考える

- ObjectをModel,Spriteにとりあえず対応させる
	- リソース (割り当てるものはそれぞれに持たせる
	- 本当にObjectがあるべきなのかを見直す(設計が甘いので練り直す)

- Pipelineを細分化
- Pipelineを複数用意(BlendMode用)

- Texture(png)の読み込みに拡張子を必要ない形に変更する
	- 同じ名前のファイルが複数あった場合どうするかを考える(example.png & example.jpg)

- デストラクタに実装されている終了処理をFinalize(shutdown)関数に置き換え明示的な呼び出しを行う

- Add Finalize(Change Destructor to Finalize)
- FontRendering
- Adjust for MultiThread
- Changeable Icon
- Add TimeScheduler(Maybe runnable system)
- Fix ParticleSystem
- Add Config (設定ファイルのようなものを作る)
- Add Scheduler (res.JavaRunnable)
- Multithreading TextureManager

### 命名
- Object3d => Model
- Model => Mesh
- 一部Draw-> Render

### AudioLoadについてのTODO
LoadからWavやmp3などの形式に応じて呼び出す関数を変更する
Typeとして定義して関数ポインタテーブルから呼び出すのもよいかも

.wavなどの拡張子の有無を関係なくtryするようにする
なかった場合Load関数でnameに.wav等をつける or key側に拡張子を付けないよう変更する

### Inputについて
現状InputをとりあえずSingletonとして使用する。
が、これでは2P対応が難しいため、Singletonではない方法を模索する必要がある。

### Component指向について
良い資料を見つけたので勉強をしつつ、実装を試行してみる

### OpenCV
よく使われているライブラリなので、使い方を覚えておくと便利かもしれない
必要に応じてexternに追加予定

### OpenGL (GLSL)
汎用性が高いらしいので、選択できるように今後変更を加える予定

### CMake
調べた感じ使いやすそうなので今後を考えて移行の準備をしておく。


# Done
- Frameworkに持たせるクラスたちを必要に応じてSingleton化する

- ParticleSystem(CG3への対応)
- ParticleCommonの追加
- Particleのクラス化前にParticle部を丸々ParticleManagerに実装しなおすこと
- ParticleCommonを追加(本当に必要か要検討)
- ~~GeometryShaderの理解と必要個所の検討~~
- Add Log Library
- Sprite用のシェーダーを用意

#### DirectXCommonを以下のように変更
- GraphicsPipelineをメンバ変数から除外(Model, Sprite, Particleにはそれぞれtypeを引数として持たせ、各Commonクラスに変数として追加)
- Shaderをメンバ変数から除外(Model, Sprite, Particleにはそれぞれtypeを引数として持たせ、作成された各GraphicsPipelineの変数として追加)
- ~~関数の引数から除外~~

### DirectXCommonははたしてSingletonにすべきなのか。
https://qiita.com/mo12ino/items/abf2e31e34278ebea42c

資料に抗ってSingletonを極力なしでやってみる。例外はあるけど(AudioやらTextureやら)
##### 例外君たち
- SpriteCommon Done
- ModelCommon Done
- TextureManager Done
- ModelManager Done
- AudioManager(Audio) Almost
- Input(?) *要検討
- Light *検討