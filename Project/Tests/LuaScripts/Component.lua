
function Component:initlua()
    print("Component:initlua() from lua")
    print("intReadWriteLua = " .. self.intReadWriteLua)
    print("intReadOnlyLua = " .. self.intReadonlyLua)

    self.intReadWriteLua = self.intReadWriteLua + 1
    self.intReadOnlyLua = self.intReadOnlyLua + 1   -- Should do nothing since readonly
end

function Component:staticFuncLua()
    print("Component:staticFuncLua() from lua")
    Component:staticFunc()
end