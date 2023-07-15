using System;

namespace PantheonEngine
{
    public abstract class NativeObject : IDisposable
    {
        protected IntPtr nativeObject;

        protected abstract void Dispose(bool disposing);

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        ~NativeObject()
        {
            Dispose(false);
        }
    }
}