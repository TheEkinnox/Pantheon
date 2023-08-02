using System.Runtime.InteropServices;
using System.Text;

namespace PantheonEngine.Input
{
    public class Input
    {
        public static (double, double) MousePosition
        {
            get
            {
                double x = 0, y = 0;
                Internal_GetMousePosition(ref x, ref y);
                return (x, y);
            }
        }

        public static (double, double) MouseDelta
        {
            get
            {
                double x = 0, y = 0;
                Internal_GetMouseDelta(ref x, ref y);
                return (x, y);
            }
        }

        public static int GetScanCode(EKey key)
        {
            return Internal_GetScanCode((int)key);
        }

        public static string GetKeyName(EKey key)
        {
            return Internal_GetKeyName((int)key, -1).ToString();
        }

        public static string GetKeyName(int scanCode)
        {
            return Internal_GetKeyName((int)EKey.Unknown, scanCode).ToString();
        }

        public static bool IsKeyUp(EKey key)
        {
            return Internal_IsKeyUp((int)key);
        }

        public static bool IsKeyDown(EKey key)
        {
            return Internal_IsKeyDown((int)key);
        }

        public static bool IsKeyPressed(EKey key)
        {
            return Internal_IsKeyPressed((int)key);
        }

        public static bool IsKeyReleased(EKey key)
        {
            return Internal_IsKeyReleased((int)key);
        }

        public static bool IsMouseButtonUp(EMouseButton mouseButton)
        {
            return Internal_IsMouseButtonUp((int)mouseButton);
        }

        public static bool IsMouseButtonDown(EMouseButton mouseButton)
        {
            return Internal_IsMouseButtonDown((int)mouseButton);
        }

        public static bool IsMouseButtonPressed(EMouseButton mouseButton)
        {
            return Internal_IsMouseButtonPressed((int)mouseButton);
        }

        public static bool IsMouseButtonReleased(EMouseButton mouseButton)
        {
            return Internal_IsMouseButtonReleased((int)mouseButton);
        }

        [DllImport(EngineLibs.PantheonCore, CallingConvention = CallingConvention.Cdecl, EntryPoint = "InputManager_isKeyUpScanCode")]
        public static extern bool IsScanCodeUp(int scanCode);

        [DllImport(EngineLibs.PantheonCore, CallingConvention = CallingConvention.Cdecl, EntryPoint = "InputManager_isKeyDownScanCode")]
        public static extern bool IsScanCodeDown(int scanCode);

        [DllImport(EngineLibs.PantheonCore, CallingConvention = CallingConvention.Cdecl, EntryPoint = "InputManager_isKeyPressedScanCode")]
        public static extern bool IsScanCodePressed(int scanCode);

        [DllImport(EngineLibs.PantheonCore, CallingConvention = CallingConvention.Cdecl, EntryPoint = "InputManager_isKeyReleasedScanCode")]
        public static extern bool IsScanCodeReleased(int scanCode);

    #region BINDINGS

        [DllImport(EngineLibs.PantheonCore, CallingConvention = CallingConvention.Cdecl, EntryPoint = "InputManager_getMousePosition")]
        private static extern void Internal_GetMousePosition(ref double x, ref double y);

        [DllImport(EngineLibs.PantheonCore, CallingConvention = CallingConvention.Cdecl, EntryPoint = "InputManager_getMouseDelta")]
        private static extern void Internal_GetMouseDelta(ref double x, ref double y);

        [DllImport(EngineLibs.PantheonCore, CallingConvention = CallingConvention.Cdecl, EntryPoint = "InputManager_getScanCode")]
        private static extern int Internal_GetScanCode(int key);

        [DllImport(EngineLibs.PantheonCore, CallingConvention = CallingConvention.Cdecl, EntryPoint = "InputManager_getScanCode")]
        private static extern StringBuilder Internal_GetKeyName(int key, int scanCode);

        [DllImport(EngineLibs.PantheonCore, CallingConvention = CallingConvention.Cdecl, EntryPoint = "InputManager_isKeyUp")]
        private static extern bool Internal_IsKeyUp(int key);

        [DllImport(EngineLibs.PantheonCore, CallingConvention = CallingConvention.Cdecl, EntryPoint = "InputManager_isKeyDown")]
        private static extern bool Internal_IsKeyDown(int key);

        [DllImport(EngineLibs.PantheonCore, CallingConvention = CallingConvention.Cdecl, EntryPoint = "InputManager_isKeyPressed")]
        private static extern bool Internal_IsKeyPressed(int key);

        [DllImport(EngineLibs.PantheonCore, CallingConvention = CallingConvention.Cdecl, EntryPoint = "InputManager_isKeyReleased")]
        private static extern bool Internal_IsKeyReleased(int key);

        [DllImport(EngineLibs.PantheonCore, CallingConvention = CallingConvention.Cdecl, EntryPoint = "InputManager_isMouseButtonUp")]
        private static extern bool Internal_IsMouseButtonUp(int mouseButton);

        [DllImport(EngineLibs.PantheonCore, CallingConvention = CallingConvention.Cdecl, EntryPoint = "InputManager_isMouseButtonDown")]
        private static extern bool Internal_IsMouseButtonDown(int mouseButton);

        [DllImport(EngineLibs.PantheonCore, CallingConvention = CallingConvention.Cdecl, EntryPoint = "InputManager_isMouseButtonPressed")]
        private static extern bool Internal_IsMouseButtonPressed(int mouseButton);

        [DllImport(EngineLibs.PantheonCore, CallingConvention = CallingConvention.Cdecl, EntryPoint = "InputManager_isMouseButtonReleased")]
        private static extern bool Internal_IsMouseButtonReleased(int mouseButton);

    #endregion
    }
}