# 段階8のNT-Shell固定情報

2026-10-31（JST）、段階7の公式配布NT-Shell 0.3.1を本段階へバイト単位でコピーした。
段階8は独立してビルドでき、段階7のソースをビルド時に参照しない。

- 作者：Shinichiro Nakamura / CuBeatSystems。
- 公式配布物：[ntshell-v0.3.1.tar.gz](https://cubeatsystems.com/ntshell/resources/ntshell-v0.3.1.tar.gz)。
- 配布一覧：[NT-Shell Downloads](https://cubeatsystems.com/ntshell/download.html)。
- SHA-256：`ea1497ce208cf9e3d02c161fd5d72e0e785b34074e31dca34fe855ba8fd89c74`。
- `core/`全14ファイル、`util/ntopt.c`と`ntopt.h`、計16ファイル。上流変更なし。
- MIT許諾・著作権表示、`vtrecv.c`のpublic domain表示を保持。
- `ntconf.h`も未変更。編集領域64 B、履歴8件。
- 配布物は0.3.1だが、`ntshell_version()`の版表記は0.2.0。起動表示はpackage 0.3.1。
- 配布アーカイブのためGitコミット／タグは未取得。

段階7で取得したローカル原本と、段階8の16ファイル・`SHA256SUMS`を照合済み。
このSHAは内容照合用であり、配布元の電子署名ではない。

```powershell
python "$learningRepo\stages\08_memory_ram\scripts\verify_vendor.py"
# 段階7で取得した原本がある場合
python "$learningRepo\stages\08_memory_ram\scripts\verify_vendor.py" `
    --archive "$learningRepo\.tmp\ntshell-v0.3.1.tar.gz"
```

元の取得経緯は[段階7の固定情報](../07_ntshell/UPSTREAM.md)を参照。
