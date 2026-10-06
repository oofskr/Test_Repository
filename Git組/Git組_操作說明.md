# HW2 Git 版本控制實作（TortoiseGit）

> 圖片請依檔名放在和本檔同一個資料夾。
> 練習用版本庫：`hw2_git_demo`
> 共 6 個操作、20 張截圖，依上台順序編號。

---

## 一、建立版本庫（Init）

![](01_init_menu.png)
![](02_init_dialog.png)
![](03_init_done.png)

- 01：在空資料夾按右鍵 →「Git 在此建立版本庫」
- 02：不勾「設為純版本庫」（要在這個資料夾裡工作）→ 確定
- 03：初始化成功，資料夾出現隱藏的 `.git` 資料夾
- 上台講法：
  - 第一步是建立 Git 版本庫，之後這個資料夾的所有變更都會被記錄
  - 所有版本紀錄都存在 `.git` 這個資料夾裡

---

## 二、提交（Commit）

![](04_commit_edit.png)
![](05_commit_menu.png)
![](06_commit_dialog.png)
![](07_commit_done.png)

- 04：新增 `README.md`，內容 `# HW2 Git Demo`
- 05：右鍵 →「Git 提交 -> "master"」
- 06：填寫提交訊息 `Initial commit: add README`，勾選要提交的檔案
- 07：提交成功，產生版本編號 `9560b9f`
- 之後再做兩次 commit：`Update README`、`Add hello.txt`
- 上台講法：
  - Commit 就是幫目前的檔案存一個版本快照
  - 每次提交都要寫訊息，說明這次改了什麼
  - 下方可以選擇這次要提交哪些檔案
  - 提交後每個版本都會有一個唯一編號

---

## 三、版本歷史（Log）

![](08_log_menu.png)
![](09_log.png)
![](10_log_actions.png)

- 08：右鍵 → TortoiseGit →「顯示日誌」
- 09：列出 3 筆 commit，含訊息、作者、日期；中間是 SHA-1；下方是該次修改的檔案與增刪行數
- 10：在任一版本按右鍵，可以從這個版本建立分支、切換到這個版本、重設到這個版本等
- 上台講法：
  - 這是版本歷史，列出每一次 commit 的訊息、作者和時間
  - 每個版本都有唯一的 SHA-1 編號
  - 點選任一筆可以看到那次改了哪些檔案、加了幾行、刪了幾行
  - 需要時也可以從歷史紀錄回到任何一個舊版本

---

## 四、建立分支（Create Branch）

![](11_branch_menu.png)
![](12_branch_dialog.png)
![](13_branch_commit.png)

- 11：右鍵 → TortoiseGit →「建立分支」
- 12：名稱 `feature`，基於 HEAD (master)，勾「切換到新分支」
- 13：在 feature 分支新增 `feature.txt`（內容 `New feature`）並 commit：`Add feature.txt`
- 上台講法：
  - 分支讓我們開發新功能時不影響主線 master
  - 多人協作時，每個人可以在自己的分支上開發，完成後再合併

---

## 五、切換分支與分岔（Switch）

![](14_switch_dialog.png)
![](15_switch_result.png)
![](16_master_edit.png)
![](17_branch_log.png)

- 14：右鍵 →「切換／取出」→ 選 `master`
- 15：切回 master 後，資料夾裡的 `feature.txt` 不見了
- 16：在 master 修改 `hello.txt`，加一行 `Hello from master` 並 commit：`Update hello.txt on master`
- 17：開啟日誌並勾「所有分支」，版本圖從 `Add hello.txt` 分岔成兩條線
- 上台講法：
  - 切回 master 後看不到 feature 的檔案，證明兩個分支互相獨立
  - 兩個分支各自往前開發，版本圖在這裡分岔

---

## 六、合併分支（Merge）

![](18_merge_menu.png)
![](19_merge_dialog.png)
![](20_log_merged.png)

- 18：確認目前在 master → 右鍵 → TortoiseGit →「合併」
- 19：選擇要合併進來的分支 `feature` → 確定
- 20：最上面多了 merge commit `Merge branch 'feature'`，版本圖呈現「分岔 → 匯合」；下方顯示 `feature.txt` 從 feature 帶進來、`hello.txt` 是 master 自己的修改，兩邊都保留
- 上台講法：
  - 功能開發完成後，把 feature 分支合併回主線 master
  - 合併後兩邊的修改都保留下來
  - 這就是團隊協作的基本流程：開分支 → 各自開發 → 合併回主線
