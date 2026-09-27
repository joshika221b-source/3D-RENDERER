#include <windows.h>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

// ============================================================
// 3D VECTOR
// ============================================================

struct Vec3
{
    float x;
    float y;
    float z;

    Vec3 operator-(const Vec3& v) const
    {
        return {x - v.x, y - v.y, z - v.z};
    }
};

// ============================================================
// VECTOR MATH
// ============================================================

float dot(Vec3 a, Vec3 b)
{
    return a.x * b.x +
           a.y * b.y +
           a.z * b.z;
}

Vec3 cross(Vec3 a, Vec3 b)
{
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

Vec3 normalize(Vec3 v)
{
    float length = sqrt(
        v.x * v.x +
        v.y * v.y +
        v.z * v.z
    );

    if (length == 0.0f)
        return {0.0f, 0.0f, 0.0f};

    return {
        v.x / length,
        v.y / length,
        v.z / length
    };
}

// ============================================================
// ROTATION
// ============================================================

Vec3 rotateX(Vec3 p, float angle)
{
    float c = cos(angle);
    float s = sin(angle);

    return {
        p.x,
        p.y * c - p.z * s,
        p.y * s + p.z * c
    };
}

Vec3 rotateY(Vec3 p, float angle)
{
    float c = cos(angle);
    float s = sin(angle);

    return {
        p.x * c + p.z * s,
        p.y,
        -p.x * s + p.z * c
    };
}

Vec3 rotateZ(Vec3 p, float angle)
{
    float c = cos(angle);
    float s = sin(angle);

    return {
        p.x * c - p.y * s,
        p.x * s + p.y * c,
        p.z
    };
}

Vec3 rotatePoint(
    Vec3 p,
    float x,
    float y,
    float z)
{
    p = rotateX(p, x);
    p = rotateY(p, y);
    p = rotateZ(p, z);

    return p;
}

// ============================================================
// TRIANGLE
// ============================================================

struct Triangle
{
    Vec3 a;
    Vec3 b;
    Vec3 c;
    COLORREF color;
};

// ============================================================
// GLOBALS
// ============================================================

float angleX = 0.0f;
float angleY = 0.0f;
float angleZ = 0.0f;

bool autoRotate = true;

vector<Vec3> cube;
enum Shape{
    CUBE, SPHERE
};
Shape currentShape = CUBE;

// ============================================================
// CREATE CUBE
// ============================================================

vector<Vec3> createCube()
{
    vector<Vec3> vertices;

    vertices.push_back({-1.0f, -1.0f, -1.0f});
    vertices.push_back({ 1.0f, -1.0f, -1.0f});
    vertices.push_back({ 1.0f,  1.0f, -1.0f});
    vertices.push_back({-1.0f,  1.0f, -1.0f});

    vertices.push_back({-1.0f, -1.0f,  1.0f});
    vertices.push_back({ 1.0f, -1.0f,  1.0f});
    vertices.push_back({ 1.0f,  1.0f,  1.0f});
    vertices.push_back({-1.0f,  1.0f,  1.0f});

    return vertices;
}

// ============================================================
// CREATE CUBE TRIANGLES
// ============================================================

vector<Triangle> createTriangles(
    const vector<Vec3>& v)
{
    vector<Triangle> t;

    // Front
    t.push_back({
        v[0], v[1], v[2],
        RGB(70, 140, 255)
    });

    t.push_back({
        v[0], v[2], v[3],
        RGB(70, 140, 255)
    });

    // Back
    t.push_back({
        v[4], v[6], v[5],
        RGB(100, 170, 255)
    });

    t.push_back({
        v[4], v[7], v[6],
        RGB(100, 170, 255)
    });

    // Left
    t.push_back({
        v[0], v[3], v[7],
        RGB(50, 110, 220)
    });

    t.push_back({
        v[0], v[7], v[4],
        RGB(50, 110, 220)
    });

    // Right
    t.push_back({
        v[1], v[5], v[6],
        RGB(90, 160, 245)
    });

    t.push_back({
        v[1], v[6], v[2],
        RGB(90, 160, 245)
    });

    // Top
    t.push_back({
        v[3], v[2], v[6],
        RGB(150, 200, 255)
    });

    t.push_back({
        v[3], v[6], v[7],
        RGB(150, 200, 255)
    });

    // Bottom
    t.push_back({
        v[0], v[4], v[5],
        RGB(35, 80, 160)
    });

    t.push_back({
        v[0], v[5], v[1],
        RGB(35, 80, 160)
    });

    return t;
}
vector<Triangle> createSphere()
{
    vector<Triangle> triangles;

    const int stacks = 20;
    const int slices = 30;

    const float radius = 2.0f;

    vector<vector<Vec3>> points(
        stacks + 1,
        vector<Vec3>(slices)
    );

    for (int i = 0; i <= stacks; i++)
    {
        float phi =
            3.14159265f * i / stacks;

        for (int j = 0; j < slices; j++)
        {
            float theta =
                2.0f * 3.14159265f * j / slices;

            points[i][j] = {
                radius * sin(phi) * cos(theta),
                radius * cos(phi),
                radius * sin(phi) * sin(theta)
            };
        }
    }

    for (int i = 0; i < stacks; i++)
    {
        for (int j = 0; j < slices; j++)
        {
            int next = (j + 1) % slices;

            Vec3 a = points[i][j];
            Vec3 b = points[i][next];
            Vec3 c = points[i + 1][j];
            Vec3 d = points[i + 1][next];

            if (i != 0)
            {
                triangles.push_back({
                    a,
                    c,
                    b,
                    RGB(80, 170, 255)
                });
            }

            if (i != stacks - 1)
            {
                triangles.push_back({
                    b,
                    c,
                    d,
                    RGB(80, 170, 255)
                });
            }
        }
    }

    return triangles;
}

// ============================================================
// 3D -> 2D PERSPECTIVE PROJECTION
// ============================================================

POINT projectPoint(
    Vec3 p,
    int width,
    int height)
{
    const float cameraDistance = 6.0f;
    const float focalLength = 650.0f;

    float denominator =
        p.z + cameraDistance;

    if (denominator < 0.1f)
        denominator = 0.1f;

    float factor =
        focalLength / denominator;

    POINT point;

    point.x =
        static_cast<LONG>(
            p.x * factor +
            width / 2.0f
        );

    point.y =
        static_cast<LONG>(
            -p.y * factor +
            height / 2.0f
        );

    return point;
}

// ============================================================
// DRAW ONE TRIANGLE
// ============================================================

void drawTriangle(
    HDC hdc,
    const Triangle& triangle,
    int width,
    int height)
{
    POINT points[3];

    points[0] =
        projectPoint(
            triangle.a,
            width,
            height
        );

    points[1] =
        projectPoint(
            triangle.b,
            width,
            height
        );

    points[2] =
        projectPoint(
            triangle.c,
            width,
            height
        );

    // Calculate surface normal
    Vec3 edge1 =
        triangle.b - triangle.a;

    Vec3 edge2 =
        triangle.c - triangle.a;

    Vec3 normal =
        normalize(
            cross(edge1, edge2)
        );

    // Light
    Vec3 light =
        normalize({
            -0.5f,
            -0.7f,
            -1.0f
        });

    float brightness =
        dot(normal, light);

    brightness =
        max(0.20f, brightness);

    brightness =
        min(1.0f, brightness);

    int r =
        static_cast<int>(
            GetRValue(triangle.color) *
            brightness
        );

    int g =
        static_cast<int>(
            GetGValue(triangle.color) *
            brightness
        );

    int b =
        static_cast<int>(
            GetBValue(triangle.color) *
            brightness
        );

    COLORREF faceColor =
        RGB(r, g, b);

    HBRUSH brush =
        CreateSolidBrush(faceColor);

    HPEN pen =
        CreatePen(
            PS_SOLID,
            2,
            RGB(220, 235, 255)
        );

    HGDIOBJ oldBrush =
        SelectObject(
            hdc,
            brush
        );

    HGDIOBJ oldPen =
        SelectObject(
            hdc,
            pen
        );

    Polygon(
        hdc,
        points,
        3
    );

    SelectObject(
        hdc,
        oldBrush
    );

    SelectObject(
        hdc,
        oldPen
    );

    DeleteObject(brush);
    DeleteObject(pen);
}

// ============================================================
// RENDER SCENE
// ============================================================

void renderScene(
    HDC hdc,
    int width,
    int height)
{
    // --------------------------------------------------------
    // Background
    // --------------------------------------------------------

    HBRUSH background =
        CreateSolidBrush(
            RGB(8, 10, 18)
        );

    RECT backgroundRect;

    backgroundRect.left = 0;
    backgroundRect.top = 0;
    backgroundRect.right = width;
    backgroundRect.bottom = height;

    FillRect(
        hdc,
        &backgroundRect,
        background
    );

    DeleteObject(background);

    // --------------------------------------------------------
    // Transform cube
    // --------------------------------------------------------

    // --------------------------------------------------------
// Create selected shape
// --------------------------------------------------------

vector<Triangle> triangles;

if (currentShape == CUBE)
{
    triangles = createTriangles(cube);
}
else
{
    triangles = createSphere();
}

// --------------------------------------------------------
// Rotate selected shape
// --------------------------------------------------------

for (size_t i = 0; i < triangles.size(); i++)
{
    triangles[i].a =
        rotatePoint(
            triangles[i].a,
            angleX,
            angleY,
            angleZ
        );

    triangles[i].b =
        rotatePoint(
            triangles[i].b,
            angleX,
            angleY,
            angleZ
        );

    triangles[i].c =
        rotatePoint(
            triangles[i].c,
            angleX,
            angleY,
            angleZ
        );
}
    // --------------------------------------------------------
    // Back-face culling
    // --------------------------------------------------------

    vector<Triangle> visible;

    for (size_t i = 0; i < triangles.size(); i++)
    {
        Triangle t =
            triangles[i];

        Vec3 edge1 =
            t.b - t.a;

        Vec3 edge2 =
            t.c - t.a;

        Vec3 normal =
            normalize(
                cross(
                    edge1,
                    edge2
                )
            );

        Vec3 cameraDirection =
            normalize({
                -t.a.x,
                -t.a.y,
                -t.a.z
            });

        if (dot(
                normal,
                cameraDirection
            ) > 0.0f)
        {
            visible.push_back(t);
        }
    }

    // --------------------------------------------------------
    // Painter's algorithm
    // --------------------------------------------------------

    sort(
        visible.begin(),
        visible.end(),
        [](const Triangle& a,
           const Triangle& b)
        {
            float za =
                (a.a.z +
                 a.b.z +
                 a.c.z) / 3.0f;

            float zb =
                (b.a.z +
                 b.b.z +
                 b.c.z) / 3.0f;

            return za > zb;
        }
    );

    // --------------------------------------------------------
    // Draw
    // --------------------------------------------------------

    for (size_t i = 0;
         i < visible.size();
         i++)
    {
        drawTriangle(
            hdc,
            visible[i],
            width,
            height
        );
    }

    // --------------------------------------------------------
    // Text
    // --------------------------------------------------------

    SetBkMode(
        hdc,
        TRANSPARENT
    );

    SetTextColor(
        hdc,
        RGB(225, 235, 255)
    );

    HFONT font =
        CreateFontA(
            24,
            0,
            0,
            0,
            FW_BOLD,
            FALSE,
            FALSE,
            FALSE,
            DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY,
            DEFAULT_PITCH,
            "Consolas"
        );

    HGDIOBJ oldFont =
        SelectObject(
            hdc,
            font
        );

    const char title[] =
        "C++ SOFTWARE 3D RENDERER";

    TextOutA(
        hdc,
        25,
        20,
        title,
        static_cast<int>(
            sizeof(title) - 1
        )
    );

    SetTextColor(
        hdc,
        RGB(150, 170, 200)
    );

    const char controls[] =
        "W/S: X   A/D: Y   Q/E: Z";

    TextOutA(
        hdc,
        25,
        50,
        controls,
        static_cast<int>(
            sizeof(controls) - 1
        )
    );

    const char controls2[] =
    "1: Cube    2: Sphere    R: Reset    SPACE: Auto Rotate    ESC: Exit";

    TextOutA(
        hdc,
        25,
        73,
        controls2,
        static_cast<int>(
            sizeof(controls2) - 1
        )
    );

    SelectObject(
        hdc,
        oldFont
    );

    DeleteObject(font);
}

// ============================================================
// WINDOW PROCEDURE
// ============================================================

LRESULT CALLBACK WindowProc(
    HWND hwnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (message)
    {
        case WM_CREATE:
        {
            SetTimer(
                hwnd,
                1,
                16,
                NULL
            );

            return 0;
        }

        case WM_TIMER:
        {
            if (autoRotate)
            {
                angleY += 0.025f;
                angleX += 0.008f;
            }

            InvalidateRect(
                hwnd,
                NULL,
                TRUE
            );

            return 0;
        }

        case WM_KEYDOWN:
        {
            if (wParam == 'W')
                angleX -= 0.12f;

            if (wParam == 'S')
                angleX += 0.12f;

            if (wParam == 'A')
                angleY -= 0.12f;

            if (wParam == 'D')
                angleY += 0.12f;

            if (wParam == 'Q')
                angleZ -= 0.12f;

            if (wParam == 'E')
                angleZ += 0.12f;

            if (wParam == 'R')
            {
                angleX = 0.0f;
                angleY = 0.0f;
                angleZ = 0.0f;
            }

            if (wParam == VK_SPACE)
                autoRotate = !autoRotate;
            if (wParam == '1')
                currentShape = CUBE;
            if (wParam == '2')
                currentShape = SPHERE;

            if (wParam == VK_ESCAPE)
                DestroyWindow(hwnd);

            return 0;
        }
        case WM_PAINT:
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    RECT rect;
    GetClientRect(hwnd, &rect);

    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    // Create an off-screen DC
    HDC memDC = CreateCompatibleDC(hdc);

    // Create an off-screen bitmap
    HBITMAP memBitmap =
        CreateCompatibleBitmap(
            hdc,
            width,
            height
        );

    HGDIOBJ oldBitmap =
        SelectObject(
            memDC,
            memBitmap
        );

    // Draw the entire scene off-screen
    renderScene(
        memDC,
        width,
        height
    );

    // Copy completed frame to window
    BitBlt(
        hdc,
        0,
        0,
        width,
        height,
        memDC,
        0,
        0,
        SRCCOPY
    );

    // Clean up
    SelectObject(
        memDC,
        oldBitmap
    );

    DeleteObject(memBitmap);
    DeleteDC(memDC);

    EndPaint(hwnd, &ps);

    return 0;
}

        

        case WM_DESTROY:
        {
            KillTimer(
                hwnd,
                1
            );

            PostQuitMessage(0);

            return 0;
        }
    }

    return DefWindowProcA(
        hwnd,
        message,
        wParam,
        lParam
    );
}

// ============================================================
// MAIN
// ============================================================

int main()
{
    cube =
        createCube();

    const char CLASS_NAME[] =
        "My3DRendererWindow";

    HINSTANCE hInstance =
        GetModuleHandleA(NULL);

    // --------------------------------------------------------
    // Window class
    // --------------------------------------------------------

    WNDCLASSA wc = {};

    wc.lpfnWndProc =
        WindowProc;

    wc.hInstance =
        hInstance;

    wc.lpszClassName =
        CLASS_NAME;

    wc.hCursor =
        LoadCursor(
            NULL,
            IDC_ARROW
        );

    RegisterClassA(&wc);

    // --------------------------------------------------------
    // Create window
    // --------------------------------------------------------

    HWND hwnd =
        CreateWindowExA(
            0,
            CLASS_NAME,
            "C++ 3D Renderer - From Scratch",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            1000,
            700,
            NULL,
            NULL,
            hInstance,
            NULL
        );

    if (hwnd == NULL)
        return 0;

    ShowWindow(
        hwnd,
        SW_SHOW
    );

    UpdateWindow(hwnd);

    // --------------------------------------------------------
    // Message loop
    // --------------------------------------------------------

    MSG msg = {};

    while (
        GetMessageA(
            &msg,
            NULL,
            0,
            0
        ) > 0
    )
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return 0;
}