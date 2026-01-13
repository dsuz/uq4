こちらではアンリアルクエスト4GAME編のサンプルプロジェクトについて、以下の項目をReadMeとして記載しています。ぜひ参加前にご一読ください。

  ・サンプル内容
  ・フォルダ構成
  ・各アセットの役割
  ・軽量版の変更箇所
  ・イベント参加者の方へ


【サンプル内容】
本サンプルは「アンリアルクエスト4 ～アンリアルエンジンを使って1日で作品を作ろう！～」GAME編の参加者に向けて制作した Unreal Engine 5.0.3 のプロジェクトサンプルです。
ThirdPersonテンプレートをベースとして右クリックで狙い、左クリックで弾を発射する機能が実装されています。あなただけのゲームをこの1日で作っちゃいましょう！


【フォルダ構成】
ContentBrowser
    Splash：プロジェクト起動時のスプラッシュ画像が格納されています。操作等は不要です。
    Characters
        Mannequin_UE4
            Animations：キャラクターアニメーション、アニメーションBP、アニメーションモンタージュなどアニメーションに関するものが格納されています。
    UnrealQuest4_Game
        Audio：弾の発射の際の効果音が格納されています
        Blueprints：的のBPが格納されています。
        Maps：デフォルトのマップを格納しています。
        Materials：使用されている各種マテリアルを格納しています。
        Meshes：使用されている各種モデルデータを格納しています。
        Textures：使用されているテクスチャを格納しています。
        ThirdPerson
            Blueprints：プレイヤーBPとステート管理のためのEnumを格納しています。
        Widgets：レティクルとして使用するウィジェットBPを格納しています。


【各アセットの役割】
ブループリント
    BP_GameMode：処理等は追加しておらず、Default Pawn Classを設定しています。
    BP_UQ4Player：Unreal Engine 4のマネキンを基にしたプレイヤーのBPです。マウス右ボタンで狙い状態に切り替え、左ボタンで弾を発射します。狙い状態の間は角度を変えられますが動けません。
    BP_Projectile：プレイヤーの発射処理によって発射される弾です。発射された際に音が鳴るようになっています。
    ABP_ThirdPerson：Unreal Engine 4のThirdPersonテンプレートに含まれるアニメーションブループリントをベースに狙い状態の時のピッチ角調整や発射用のアニメーションロジックを追加しています。

レベル（マップ）
    UE5のThirdPersonテンプレートに付属しているマップに手を加えたものです。


【軽量版の変更箇所】
今回ダウンロードされたプロジェクトが「Q4Template_Lite」だった場合。通常のプロジェクトデータと違い開発にてPCへかかる負荷を落とすため設定の変更を行っています。
以下に変更を行っている設定を列挙しています。

プロジェクト設定 > ターゲットハードウェア
  ・Target Hardware：モバイル/タブレット、スケーラブルな3D・2D
  ・Rendering -> Dynamic Global Illumination Method：None
  ・Rendering -> Reflection Method:None
  ・Rendering -> Shadow Map Method:Shadow Maps


Viewport > 左上のメニュー
  ・リアルタイム：オフ

ツールバー > 設定
  ・エンジンの拡張機能設定：中
     [projectpath]/Config/DefaultEngine.ini の [ConsoleVariables] セクションに設定している9項目が該当設定です。
     エディター上にて設定を変更する場合は.iniファイルの設定が優先されるため、ファイル上から [ConsoleVariables] セクションの9項目を削除した上で変更してください。

  ・マテリアルの品質レベル：低
     [projectpath]/Config/DefaultEngine.ini の [ConsoleVariables] セクションに設定しています。

    もし軽量版を利用しても動作が重い場合は以下をお試しください。

  ・エンジンの拡張機能設定：低
  ・描画レベルをプレビュー：Android ES3.1(※シェーダーコンパイルが多く走ります。)

コンテンツブラウザ > 表示オプション
  ・リアルタイムサムネイル：オフ
    [projectpath]/Config/DefaultEditorPerProjectUserSettings.iniの[/Script/UnrealEd.LevelEditorViewportSettings]セクションに設定しています。

FPS上限を60FPSに設定
    [projectpath]/Config/DefaultEngine.ini の [/Script/UnrealEd.EditorEngine] セクションに設定しています。
    もしもFPS上限を固定したくない場合は該当セクションをまるごと削除してください。


【独自アセットの権利について】
本プロジェクトにはUnreal Engine 5のThird Person Templateに含まれていないアセットが含まれています。
それらのアセットは株式会社ヒストリアの著作物となりますが、商用・非商用関わらずご自由に使用・改変頂いて構いません。
権利表記も不要です。

ご使用については全て自己責任でお願いいたします。
いかなる損害・損失・不利益が発生した場合も、株式会社ヒストリアでは一切の責任を負いません。
