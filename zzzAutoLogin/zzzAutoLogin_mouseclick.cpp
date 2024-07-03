#include <iostream>
#include <windows.h>
#include <ShlObj.h>

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
    int afterCancelBtn = 5000;
} sleepTime;

struct Btn {
    int x;
    int y;
};

struct GAMEWindowLocs {
    RECT gameRect;
    int size[2];
    Btn loginBtn;
    Btn cancelBtn;
    int type = 0; // 0:该点登录按钮, 1:该点取消按钮
};

// 发送鼠标点击事件
void SendMouseClick(HWND hWnd, int x, int y) {
    // 将屏幕坐标转换为客户端坐标
    POINT pt = { x, y };
    // ScreenToClient(hWnd, &pt);

    // 发送鼠标按下事件
    PostMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(pt.x, pt.y));

    // 发送鼠标松开事件
    PostMessage(hWnd, WM_LBUTTONUP, 0, MAKELPARAM(pt.x, pt.y));
}

bool upGameWindow(HWND gameHandle, GAMEWindowLocs* pGameWindowLocs) {
    if (gameHandle == NULL) {
        cout << "找不到绝区零" << endl;
        return false;
    }

    RECT gameRect = RECT(); // 储存窗口位置及大小, left,top左上角x,y right,bottom右下角x,y
    bool hadGet_GameRect = GetWindowRect(gameHandle, &gameRect); // 获取窗口位置及大小(窗口句柄, 要保存的位置) 返回非零获取成功
    if (!hadGet_GameRect) {
        cout << "无法获取窗口位置及大小" << endl;
        return false;
    }

    int size[] = { gameRect.right - gameRect.left , gameRect.bottom - gameRect.top }; // 计算窗口宽度和高度

    // 更新窗口信息
    pGameWindowLocs->gameRect = gameRect;
    pGameWindowLocs->size[0] = size[0];
    pGameWindowLocs->size[1] = size[1];
    pGameWindowLocs->loginBtn.x = int(pGameWindowLocs->size[0] * LoginRV.x + pGameWindowLocs->gameRect.left);
    pGameWindowLocs->loginBtn.y = int(pGameWindowLocs->size[1] * LoginRV.y + pGameWindowLocs->gameRect.top);
    pGameWindowLocs->cancelBtn.x = int(pGameWindowLocs->size[0] * CancelRV.x + pGameWindowLocs->gameRect.left);
    pGameWindowLocs->cancelBtn.y = int(pGameWindowLocs->size[1] * CancelRV.y + pGameWindowLocs->gameRect.top);

    cout << "Lx,y = " << gameRect.left << "," << gameRect.top << "\t" << " Rx,y = " << gameRect.right << "," << gameRect.bottom << "\t" << " w,h = " << size[0] << "," << size[1] << endl;
    return true;
}

int main() {
    cout << "绝区零自动登录!" << endl;
    BOOL bIsAdmin = IsUserAnAdmin();
    if (!bIsAdmin) {
        cout << "请以管理员运行" << endl;
        system("pause");
        return -1;
    }

    GAMEWindowLocs GameWindowLocs;

    while (TRUE) {
        WCHAR gameTitle[] = L"绝区零"; // L表示unicode宽字符，string依赖操作系统环境默认ansi窄字符
        HWND gameHandle = FindWindow(NULL, gameTitle); // 按（窗口类名，窗口标题）搜索窗口句柄, 失败时返回NULL
        if (gameHandle == NULL) {
            cout << "找不到绝区零，请确认绝区零正在运行" << endl;
            return 1;
        }
        bool hadGet_GameRect = upGameWindow(gameHandle, &GameWindowLocs);
        SetForegroundWindow(gameHandle); // 调到前台
        if (!hadGet_GameRect) {
            cout << "获取坐标错误" << endl;
            Sleep(1000);
            continue;
        }

        switch (GameWindowLocs.type) {
        case 0: // 点击登录按钮, 坐标(GameWindowLocs.loginBtn.x,GameWindowLocs.loginBtn.y)
            cout << "点击了登录按钮" << " Login: " << GameWindowLocs.loginBtn.x << "," << GameWindowLocs.loginBtn.y << endl;
            //SendMouseClick(gameHandle, GameWindowLocs.loginBtn.x, GameWindowLocs.loginBtn.y); // 发送消息事件不管事
            SetCursorPos(GameWindowLocs.loginBtn.x, GameWindowLocs.loginBtn.y);
            mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, GameWindowLocs.loginBtn.x, GameWindowLocs.loginBtn.y, 0, 0);
            Sleep(sleepTime.afterLoginBtn);
            break;
        case 1: // 点击取消按钮, 坐标(GameWindowLocs.cancelBtn.x,GameWindowLocs.cancelBtn.y)
            cout << "点击了取消按钮" << " Cancel: " << GameWindowLocs.cancelBtn.x << "," << GameWindowLocs.cancelBtn.y << endl;
            //SendMouseClick(gameHandle, GameWindowLocs.cancelBtn.x, GameWindowLocs.cancelBtn.y);
            SetCursorPos(GameWindowLocs.cancelBtn.x, GameWindowLocs.cancelBtn.y);
            mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, GameWindowLocs.cancelBtn.x, GameWindowLocs.cancelBtn.y, 0, 0);
            Sleep(sleepTime.afterCancelBtn);
            break;
        }
        GameWindowLocs.type = (GameWindowLocs.type + 1) % 2;
        Sleep(1000);
    }
    system("pause");
    return 0;
}
