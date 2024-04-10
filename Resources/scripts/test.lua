local tmp = {
    max_tick = 0
}

local req = {
    [1] = require("testRequired"),
    [2] = require("testRequired.lua"),
    [3] = require("scripts.testRequired"),
    [4] = require("scripts/testRequired"),
    [5] = require("scripts.testRequired.lua"),
    [6] = require("scripts/testRequired.lua"),
    [7] = require("assets.scripts.testRequired"),
    [8] = require("assets.scripts/testRequired"),
    [9] = require("assets/scripts.testRequired"),
    [10] = require("assets/scripts/testRequired"),
    [11] = require("assets.scripts.testRequired.lua"),
    [12] = require("assets.scripts/testRequired.lua"),
    [13] = require("assets/scripts.testRequired.lua"),
    [14] = require("assets/scripts/testRequired.lua"),
}

local tick_count = 0
local time = 0

local fixed_tick_count = 0
local fixed_time = 0

local function round(x)
    return x >= 0 and math.floor(x + 0.5) or math.ceil(x - 0.5)
end

function tmp:onInit()
    tick_count = 0
    self.max_tick = math.random(1, 100)
    print("Entity " .. self.owner .. " now has " .. self.max_tick .. " frames to live. Enjoy!")
end

function tmp:onStart()
    print("Entity " .. self.owner .. ", reporting for duty!")
end

function tmp:onUpdate(deltaTime)
    tick_count = tick_count + 1
    time = time + deltaTime

    if tick_count < self.max_tick then
        print("Tick " .. tick_count .. "! Owner: " .. self.owner .. " | dt: " .. deltaTime .. "s (" .. round(1 / deltaTime) .. "fps)")
    elseif tick_count == self.max_tick then
        print("It's entity " .. self.owner .. "'s last tick!!!")

        for i = 1, #req do
            print("= Call func of require " .. i .. " =")
            req[i]:someFunc()
        end

        self:onDestroy() -- TODO: Actually destroy the entity
    elseif tick_count == self.max_tick + 1 then
        --assert(false, "Not sur I want to give the user access to that...")
    end
end

function tmp:onFixedUpdate(deltaTime)
    fixed_tick_count = fixed_tick_count + 1
    fixed_time = fixed_time + deltaTime
end

function tmp:onDestroy()
    print("Destroyed test script of entity " .. self.owner .. " after " .. tick_count .. " ticks (" .. time .. "s) & " ..
            fixed_tick_count .. " fixed ticks (" .. fixed_time .. "s)")
end

return tmp