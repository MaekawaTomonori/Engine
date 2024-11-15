# 命名
- Object3d => Model
- Model => Mesh


# TODO
- Engine部をまとめる

- 命名変更　一部Draw-> Render

- Pipelineを細分化
- Sprite用のシェーダーを用意

- ParticleSystem(CG3への対応)

- Frameworkに持たせるクラスたちを必要に応じてSingleton化する AlmostDone

- デストラクタに実装されている終了処理をFinalize(shutdown)関数に置き換え明示的な呼び出しを行う

### DirectXCommonを以下のように変更
- GraphicsPipelineをメンバ変数から除外(Model, Sprite, Particleにはそれぞれtypeを引数として持たせ、各Commonクラスに変数として追加)
- Shaderをメンバ変数から除外(Model, Sprite, Particleにはそれぞれtypeを引数として持たせ、作成された各GraphicsPipelineの変数として追加)
- 引数でDirectXCommonを持っている関数から該当する引数を除外


## DirectXCommonははたしてSingletonにすべきなのか。
https://qiita.com/mo12ino/items/abf2e31e34278ebea42c
資料に抗ってSingletonを極力なしでやってみる。例外はあるけど(AudioやらTextureやら)
### 例外君たち
- SpriteCommon Done
- ModelCommon Done
- TextureManager Done
- ModelManager Done
- AudioManager(Audio) Almost
- Input(?) *要検討
- Light *検討



## AudioLoadについてのTODO
LoadからWavやmp3などの形式に応じて呼び出す関数を変更する
Typeとして定義して関数ポインタテーブルから呼び出すのもよいかも

.wavなどの拡張子の有無を関係なくtryするようにする
なかった場合Load関数でnameに.wav等をつける or key側に拡張子を付けないよう変更する

## Inputについて
現状InputをとりあえずSingletonとして使用する。
が、これでは2P対応が難しいため、Singletonではない方法を模索する必要がある。

## Component指向について
良い資料を見つけたので勉強をしつつ、実装を試行してみる