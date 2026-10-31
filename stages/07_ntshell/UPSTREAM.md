# NT-Shellの取得元と固定情報

取得日：2026-10-30（JST）。実施者：Codex。

- 作者：Shinichiro Nakamura / CuBeatSystems。
- 公式配布一覧：<https://cubeatsystems.com/ntshell/download.html>。
- 採用配布物：**ntshell-v0.3.1.tar.gz**（配布一覧では2016-04-30公開）。
- 取得URL：<https://cubeatsystems.com/ntshell/resources/ntshell-v0.3.1.tar.gz>。
- 配布物SHA-256：`ea1497ce208cf9e3d02c161fd5d72e0e785b34074e31dca34fe855ba8fd89c74`。
- 取り込み方法：公式アーカイブの`src/lib/core/`全14ファイルと`src/lib/util/ntopt.c`、`ntopt.h`を、`third_party/ntshell/`へコピーした。合計16ファイルをバイト単位で照合し、すべて一致した。
- Gitタグ／コミット：アーカイブで配布されているため未取得。別のGitミラーのコミットを公式配布物の識別子として使わない。
- ライセンス：各ファイル先頭のMIT許諾・著作権表示を保持。`vtrecv.c`内のVT解析コードのpublic domain表示もそのまま保持した。
- 上流変更：なし。改行のGit変換も`.gitattributes`で無効にした。`ntconf.h`も未変更（編集領域64 B、履歴8件）。
- 未採用：`ntstdio.c/.h`、サンプル用ボードソース。段階7のUART出力はアプリ側で実装する。

配布パッケージは0.3.1だが、その中の`ntshell.c`の`VERSION_*`は0.2.0のままである。
`ntshell_version()`の戻り値とパッケージ版を混同しない。アプリの起動表示は「package 0.3.1」としている。

`third_party/ntshell/SHA256SUMS`に取り込みファイルのSHA-256を保存した。
Python標準ライブラリだけで検証できる。

```powershell
python "$learningRepo\stages\07_ntshell\scripts\verify_vendor.py"
# 公式アーカイブも用意した場合は、そのSHAと各エントリも照合する。
python "$learningRepo\stages\07_ntshell\scripts\verify_vendor.py" `
    --archive "$learningRepo\.tmp\ntshell-v0.3.1.tar.gz"
```

元のアーカイブはGit管理外の`.tmp/`へ保存した。ビルドは取り込み済みソースを使用し、ネットワーク取得を行わない。
ここに記録したSHAは取得した内容の固定・照合用であり、配布元が公開した署名として扱わない。

APIの参照：<https://cubeatsystems.com/ntshell/api.html>。
`ntshell_execute()`は戻らず、readの戻り値を確認せずに受信変数を使用する。
そのため、アプリのreadコールバックは要求されたバイトを受け取るまで待つ。
