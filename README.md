#  Engine

[![Debug Build](https://github.com/MaekawaTomonori/Engine/actions/workflows/DebugBuild.yml/badge.svg)](https://github.com/MaekawaTomonori/Engine/actions/workflows/DebugBuild.yml)
[![Release Build](https://github.com/MaekawaTomonori/Engine/actions/workflows/ReleaseBuild.yml/badge.svg)](https://github.com/MaekawaTomonori/Engine/actions/workflows/ReleaseBuild.yml)

## NGNL

## 特徴
- **高度なレンダリング機能**  
  最先端の DirectX 技術を活用し、リアルタイム描画を実現。
- **柔軟なシーン管理**  
  シーン構築・変更が容易なファクトリー設計を採用。
- **オーディオ統合**  
  効果音、BGM などの再生管理機能を実装。
- **CI/CD 統合**  
  GitHub Actions による自動ビルド・テスト環境を完備。

## プロジェクト構成
- **.github/workflows/**  
  CI/CD 用の設定ファイル  
  - `DebugBuild.yml` - デバッグビルドの自動化  
  - `ReleaseBuild.yml` - リリースビルドの自動化  


- **Project/Engine/**  
  エンジン本体およびゲーム関連のコード多数を含む  
  - **src/** - 主なエンジン処理の実装  
  - **Game/** - ゲームロジックおよびシーン管理  
  - `DirectXEngine.sln` - Visual Studio ソリューションファイル 


- **Assets/**  
  使用する各種リソース（データ、フォント、シェーダー、サウンド等）

- **memo.md**  
  プロジェクトに関する各種メモや TODO を管理
