
function Component:initlua()
    print("Component:initlua() from lua")
    print("intReadWriteLua = " .. self.intReadWriteLua)
    print("intReadOnlyLua = " .. self.intReadonlyLua)
    print("defaultInt = " .. self.defaultInt)

    self:defaultMethod()

    self.intReadWriteLua = self.intReadWriteLua + 1
    self.defaultInt = self.defaultInt + 2
end

function Component:staticFuncLua()
    print("Component:staticFuncLua() from lua")
    Component:staticFunc()
end