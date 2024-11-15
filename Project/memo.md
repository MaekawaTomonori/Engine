# 命名
- Object3d => Model
- Model => Mesh

# TODO
- Engine部をまとめる

- 命名変更　一部Draw-> Render

- Pipelineを細分化
- Sprite用のシェーダーを用意

- ParticleSystem

-  Frameworkに持たせるクラスたちを必要に応じてSingleton化する

### DirectXCommonをSingleton化するに伴って、以下を変更
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
- AudioManager(Audio) TODO
- Light *検討