local Test = {
    instances = 0
}

local call_count = 0

function Test:someFunc()
    self.instances = self.instances + 1
    call_count = call_count + 1
    print("Working - Call number " .. call_count .. " | Instance number " .. self.instances)
end

return Test