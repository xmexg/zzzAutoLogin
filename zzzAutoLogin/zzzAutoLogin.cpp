#include <iostream>
#include <windows.h>

using namespace std; // std命名空间

// 登录按钮相对窗口偏移
const struct LoginRateValues {
    float x = 0.5f;
    float y = 0.83f;
} LoginRV;

// 取消按钮相对窗口偏移
const struct CancelRateValues {
    float x = 0.43f;
    float y = 0.62f;
} CancelRV;

// 冷却时间
const struct SleepTime {
    int afterLoginBtn = 1000;
    int afterCancelBtn = 3700;
} sleepTime;

struct Btn {
    int x;
    int y;
} loginBtn, cancelBtn;

// 存放窗口位置,尺寸,登录和取消按钮位置,现在该登录还是该取消
struct GAMEWindowLocs {
    RECT gameRect;
    int size[2];
    Btn loginBtn;
    Btn cancelBtn;
    int type = 0; // 0:该点登录按钮, 1:该点取消按钮
} GameWindowLocs;

boolean upGameWindow() {
    WCHAR gameTitle[] = L"绝区零"; // L表示unicode宽字符，string依赖操作系统环境默认ansi窄字符
    HWND gameHandle = FindWindow(NULL, gameTitle); // 按（窗口类名，窗口标题）搜索窗口句柄, 失败时返回NULL
    if (gameHandle == NULL) {
        cout << "找不到绝区零" << endl;
        return 1;
    }
    SetForegroundWindow(gameHandle); // 把绝区零调到前台
    RECT gameRect = RECT(); // 储存窗口位置及大小, left,top左上角x,y right,bottom右下角x,y
    bool hadGet_GameRect = GetWindowRect(gameHandle, &gameRect); // 获取窗口位置及大小(窗口句柄, 要保存的位置) 返回非零获取成功
    if (!hadGet_GameRect) {
        cout << "无法获取窗口位置及大小" << endl;
        return 2;
    }
    int size[] = { gameRect.right - gameRect.left , gameRect.bottom - gameRect.top}; // 计算窗口宽度和高度
    // 更新窗口信息
    GameWindowLocs.gameRect = gameRect;
    GameWindowLocs.size[0] = size[0];
    GameWindowLocs.size[1] = size[1];
    GameWindowLocs.loginBtn.x = int(GameWindowLocs.size[0] * LoginRV.x + GameWindowLocs.gameRect.left);
    GameWindowLocs.loginBtn.y = int(GameWindowLocs.size[1] * LoginRV.y + GameWindowLocs.gameRect.top);
    GameWindowLocs.cancelBtn.x = int(GameWindowLocs.size[0] * CancelRV.x + GameWindowLocs.gameRect.left);
    GameWindowLocs.cancelBtn.y = int(GameWindowLocs.size[1] * CancelRV.y + GameWindowLocs.gameRect.top);
    cout << "Lx,y = " << gameRect.left << "," << gameRect.top << "\t" << " Rx,y = " << gameRect.right << "," << gameRect.bottom << "\t" << " w,h = " << size[0] << "," << size[1] << endl;
    return 0;
}

int main()
{
    cout << "绝区零自动登录!" << endl;
    while (TRUE) {
        bool hadGet_GameRect = upGameWindow();
        if (hadGet_GameRect) {
            cout << "获取坐标错误" << endl;
            continue;
        }
        switch (GameWindowLocs.type) {
            case 0: // 点击登录按钮, 坐标(GameWindowLocs.loginBtn.x,GameWindowLocs.loginBtn.y)
                cout << "点击登录按钮" << " Login: " << GameWindowLocs.loginBtn.x << "," << GameWindowLocs.loginBtn.y << endl;
                //mouse_event(MOUSEEVENTF_LEFTDOWN, GameWindowLocs.loginBtn.x, GameWindowLocs.loginBtn.y, 0, 0);
                mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, GameWindowLocs.loginBtn.x, GameWindowLocs.loginBtn.y, 0, 0);
                ::SetCursorPos(GameWindowLocs.loginBtn.x, GameWindowLocs.loginBtn.y);
                Sleep(sleepTime.afterLoginBtn);
                break;
            case 1: // 点击取消按钮, 坐标(GameWindowLocs.cancelBtn.x,GameWindowLocs.cancelBtn.y)
                cout << "点击取消按钮" << " Cancel: " << GameWindowLocs.cancelBtn.x << "," << GameWindowLocs.cancelBtn.y << endl;
                mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, GameWindowLocs.cancelBtn.x, GameWindowLocs.cancelBtn.y, 0, 0);
                //mouse_event(MOUSEEVENTF_LEFTUP, GameWindowLocs.cancelBtn.x, GameWindowLocs.cancelBtn.y, 0, 0);
                //::SetCursorPos(GameWindowLocs.cancelBtn.x, GameWindowLocs.cancelBtn.y);
                SetCursorPos(10, 10);
                Sleep(sleepTime.afterCancelBtn);
                break;
        }
        SetCursorPos(100, 10);
        GameWindowLocs.type = (GameWindowLocs.type + 1) % 2;
        Sleep(1000);
        cout << "一步完成" << endl;
    }
    return 0;
}

