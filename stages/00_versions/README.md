# 段階0：使用バージョンと開発環境の方針

## 目的

後続の段階で同じビルド・検証を再現できるように、ツール、依存関係、接続方法を決めます。この段階ではサンプル実装を行いません。

## 状態と選定結果

2026-10-10：段階0のPC環境調査・バージョン選定・記録を完了しました。

Windows 11 Pro 25H2のネイティブ環境、PowerShell 7.6.5、VS Code 1.139.1を採用します。初期ビルドターゲットは`frdm_mcxn947/mcxn947/cpu0`です。

| 項目 | 採用版 |
|---|---|
| Zephyr／SDK | v4.4.2（コミット固定）／1.0.1 GNU Arm |
| Python／west | CPython 3.12.10 x64／1.5.0（専用venv） |
| CMake／Ninja | 3.31.8／1.13.1 |
| Git／LinkServer | 既存2.52.0.windows.1／既存1.4.85 |
| シリアル端末 | 既存Tera Term 5.4.0 |

選定理由、PCの実測、公式資料、機材接続、依存関係の固定方法と変更方針は[versions.md](versions.md)に記録しました。Python 3.12.10はWindowsインストーラのある版を選んだもので、最新のセキュリティ修正版ではありません。理由と変更方針も同書に記載しています。

## 手順・期待する結果・確認範囲

この段階の手順は、PCの既存ツールを読み取り調査し、採用Zephyrのタグ付きソースでボード・LCD・カメラ対応を確認して記録することです。期待する成果は、使用版、理由、取得・接続方法、未確認事項が明確になっていることです。この記録を作成し、段階0の完了条件を満たしました。

段階1へ引き継ぐ環境取得・ビルド・書き込み・デバッグのコマンドと実行ディレクトリは、[versions.mdのワークスペースと手順](versions.md#依存関係とワークスペース)に記載しています。段階0では実行していません。段階1ではHello Worldのコンソール出力とGDBでの停止・ステップ実行を確認します。

段階0時点の未確認：ホストツールの新規導入、Python依存解決、SDK使用パス、基板リビジョン、COM番号・USBドライバ、MCU-Link実装版、ビルド・実機動作。資料での対応確認と実機確認を区別しています。

段階1で環境導入・依存固定・COM5とMCU-Link V3.128の検出・Hello Worldのビルド／書き込み／UART／GDB確認を完了しました。[段階1の実測結果と訂正事項](../01_hello_world/README.md)を参照してください。基板の印刷リビジョンは引き続き未確認です。

2026-10-10：イシュー #1の完了条件4項目を記録と照合し、作業内容・完了条件をチェック済みに更新して、完了（completed）としてクローズしました。

## 作業内容

- [x] Windowsネイティブ環境を基本として、使用するエディタとデバッグ手段を決める。
- [x] FRDM-MCXN947、LCD_PAR_S035、OV7670への対応を確認してZephyrのタグ／コミットを選ぶ。
- [x] Zephyr SDK、Python、west、CMake、Ninja、LinkServerの使用バージョンを決める。
- [x] 段階14でSlintとZephyrの互換性を再確認する方針を記録する。
- [x] stages/00_versions/versions.mdに選定理由、公式資料、依存モジュール固定方法を記録する。

## 完了条件

- [x] 各ツールとZephyrのバージョン、および選定理由が記録されている。
- [x] 必要機材・接続方法・環境構築方針が記録されている。
- [x] 後の段階でバージョンを変更するときの記録方針が決まっている。
- [x] READMEに使用バージョン、後続のビルド／実行手順への参照、期待する結果、確認結果と未確認事項を記録した。

## 成果物

- [versions.md](versions.md)：選定したバージョン、理由、接続方法、変更方針。
- [対応するGitHubイシュー #1](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/1)：環境を選定した根拠と確認結果。

## 参考資料

- [FRDM-MCXN947](https://docs.zephyrproject.org/latest/boards/nxp/frdm_mcxn947/doc/index.html)
- [Getting Started](https://docs.zephyrproject.org/latest/develop/getting_started/)
