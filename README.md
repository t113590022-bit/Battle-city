## 編譯注意事項

若使用新版 CMake / CLion 重新載入專案時出現以下錯誤：

```text
Compatibility with CMake < 3.5 has been removed from CMake.
```

請在 CLion 的 CMake Options 加入：

```text
-DCMAKE_POLICY_VERSION_MINIMUM=3.5
```

設定位置：

```text
File → Settings → Build, Execution, Deployment → CMake → CMake options
```

加入後請重新 Reload CMake Project。若仍無法正常編譯，可刪除 `cmake-build-debug/` 後再重新 Reload。

## 可選：調整遊戲視窗大小

若遊戲視窗在執行時顯示為 `1280 x 720`，但希望畫面較小、方便展示或錄影，可自行修改 PTSD framework 的 config 視窗設定。

建議調整為：

```text
Width  = 1000
Height = 500
```

此設定僅影響視窗顯示大小，不影響遊戲邏輯、關卡內容、碰撞判定與操作方式。
