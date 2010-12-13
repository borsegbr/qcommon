md ..\..\qgl_pack\src\qgl
md ..\..\qgl_pack\inc\qgl
md ..\..\qgl_pack\ws\msvc.8
md ..\..\qgl_pack\ws\msvc.70
md ..\..\qgl_pack\ws\common
xcopy /E ..\..\src\qgl ..\..\qgl_pack\src\qgl
xcopy /E ..\..\inc\qgl ..\..\qgl_pack\inc\qgl
xcopy /E msvc.8 ..\..\qgl_pack\ws\msvc.8
xcopy /E msvc.70 ..\..\qgl_pack\ws\msvc.70
xcopy /E common ..\..\qgl_pack\ws\common