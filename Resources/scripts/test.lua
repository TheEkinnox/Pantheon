local Test = {
    max_tick = 0
}

local req = {
    [1] = require("testRequired"),
    [2] = require("scripts.testRequired"),
    [3] = require("scripts/testRequired"),
    [4] = require("assets.scripts.testRequired"),
    [5] = require("assets.scripts/testRequired"),
    [6] = require("assets/scripts.testRequired"),
    [7] = require("assets/scripts/testRequired")
}

local tick_count = 0
local time = 0

local fixed_tick_count = 0
local fixed_time = 0

local function round(x)
    return x >= 0 and math.floor(x + 0.5) or math.ceil(x - 0.5)
end

function Test:onInit()
    tick_count = 0
    self.max_tick = math.random(1, 100)
    print("Entity " .. self.owner .. " now has " .. self.max_tick .. " frames to live. Enjoy!")
end

function Test:onStart()
    print("Entity " .. self.owner .. ", reporting for duty!")
end

function Test:onUpdate(deltaTime)
    tick_count = tick_count + 1
    time = time + deltaTime

    if tick_count == self.max_tick then
        local avgFrameTime = time / tickCount
        print("It's entity " .. self.owner .. "'s last tick!!! Average frame time: " .. avgFrameTime .. "s (" .. Round(1 / avgFrameTime) .. "fps)")

        for i = 1, #req do
            print("= Call func of require " .. i .. " =")
            req[i]:someFunc()
        end

        -- TODO: Actually destroy the entity
    elseif tick_count == self.max_tick + 1 then
        --assert(false, "Not sur I want to give the user access to that...")
    end
end

function Test:onFixedUpdate(deltaTime)
    fixed_tick_count = fixed_tick_count + 1
    fixed_time = fixed_time + deltaTime
end

function Test:OnStop()
    print("Entity " .. self.owner .. " ready to stop")
end

function Test:onDestroy()
    local avgFrameTime = time / tick_count
    print("Destroyed test script of entity " .. self.owner .. " after " .. tick_count .. " ticks (" .. time .. "s) & " ..
            fixed_tick_count .. " fixed ticks (" .. fixed_time .. "s)" ..
            "Average frame time: " .. avgFrameTime .. "s (" .. round(1 / avgFrameTime) .. "fps)")
end

return Test