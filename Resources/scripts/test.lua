local Test = {
    max_tick = 0,
    test_bool = false
}

local req = {
    [1] = require("scripts.testRequired"),
    [2] = require("scripts/testRequired"),
    [3] = require("assets.scripts.testRequired"),
    [4] = require("assets.scripts/testRequired"),
    [5] = require("assets/scripts.testRequired"),
    [6] = require("assets/scripts/testRequired")
}

local fixed_tick_count = 0
local fixed_time = 0

local tick_count = 0
local time = 0

local function round(x)
    return x >= 0 and math.floor(x + 0.5) or math.ceil(x - 0.5)
end

local function testScene(scene)
    local typeName = Scene.__type.name

    assert(scene)
    assert(type(scene) == "userdata", "Expected \"userdata\" - Received \"" .. type(scene) .. "\"")
    assert(scene.__type.name == typeName, "Expected \"" .. typeName .. "\" - Received \"" .. scene.__type.name .. "\"")

    local entity = scene:create()

    typeName = Entity.__type.name

    assert(type(entity) == "userdata", "Expected \"userdata\" - Received \"" .. type(entity) .. "\"")
    assert(entity.__type.name == typeName, "Expected \"" .. typeName .. "\" - Received \"" .. entity.__type.name .. "\"")
    assert(scene:isValid(entity))
    assert(scene:contains(entity))

    local found = false

    for _, sceneEntity in ipairs(scene.entities) do
        found = found or sceneEntity == entity
    end

    assert(found)

    local tag = entity:getOrCreate("Tag")
    tag.value = "Entity " .. entity

    local copy = scene:create(entity)

    assert(copy)
    assert(type(copy) == "userdata", "Expected \"userdata\" - Received \"" .. type(copy) .. "\"")
    assert(copy.__type.name == typeName, "Expected \"" .. typeName .. "\" - Received \"" .. copy.__type.name .. "\"")
    assert(copy.isValid)
    assert(copy ~= entity)

    assert(copy:get("Tag").value == tag.value)

    scene:destroy(copy)

    assert(copy)
    assert(not copy.isValid)

    scene:destroy(entity)
end

local function testComponents(entity)
    assert(entity.componentCount == 1)

    local componentType = "Transform"

    assert(not entity:has(componentType))
    assert(not entity:get(componentType).isValid)

    local transform = entity:getOrCreate(componentType)

    assert(transform.isValid)
    assert(transform.__type.name == Transform.__type.name)
    assert(entity:has(componentType))
    assert(entity.componentCount == 2)

    local get = entity:get(componentType)
    assert(get.isValid)
    assert(get == transform)

    local startPos = transform.position
    local newPos = startPos + Vector3.new(1, 2, 3)

    transform.position = newPos

    assert(transform.position ~= startPos)
    assert(transform.position == newPos)
    assert(get.position == transform.position)
end

function Test.ping()
    return "pong"
end

local function testScripts(entity)
    assert(not entity:hasScript("scripts.test"))
    assert(not entity:getScript("scripts.test").isValid)

    local script = entity:addScript("scripts.test")

    assert(script.isValid)
    assert(script.owner == entity)
    assert(entity:hasScript("scripts.test"))
    assert(script.ping() == "pong")

    local invalid = entity:addScript("scripts.test")
    assert(not invalid.isValid)

    local getTest = entity:getScript("scripts.test")

    assert(getTest.isValid)
    assert(getTest == script)

    assert(not script.test_bool)
    assert(not getTest.test_bool)

    getTest.test_bool = true

    assert(getTest.test_bool)
    assert(script.test_bool)

    entity:removeScript("scripts.test")
    assert(not script.isValid)
end

local function testHierarchy(entity)
    assert(not entity.parent.isValid)
    assert(entity.root == entity)
    assert(not entity.nextSibling.isValid)
    assert(not entity.previousSibling.isValid)
    assert(entity.childCount == 0)
    assert(not entity:getChild(5).isValid)
    assert(#entity.children == 0)

    local child1 = entity:copy()
    local child2 = entity:copy()

    child1.parent = entity

    assert(child1.parent.isValid)
    assert(child1.parent == entity)
    assert(child1.root == entity)
    assert(entity.childCount == 1)
    assert(#entity.children == 1)
    assert(entity:getChild(0).isValid)
    assert(entity:getChild(0) == child1)

    child2.parent = entity

    assert(child1.previousSibling.isValid)
    assert(child1.previousSibling == child2)
    assert(not child1.nextSibling.isValid)

    assert(child2.nextSibling.isValid)
    assert(child2.nextSibling == child1)
    assert(not child2.previousSibling.isValid)
end

local function testECS(entity)
    local typeName = Entity.__type.name

    assert(type(entity) == "userdata", "Expected \"userdata\" - Received \"" .. type(entity) .. "\"")
    assert(entity.__type.name == typeName, "Expected \"" .. typeName .. "\" - Received \"" .. entity.__type.name .. "\"")
    assert(entity.isValid)

    testScene(entity.scene)

    local copy = entity:copy()

    assert(copy)
    assert(type(copy) == "userdata", "Expected \"userdata\" - Received \"" .. type(copy) .. "\"")
    assert(copy.__type.name == typeName, "Expected \"" .. typeName .. "\" - Received \"" .. copy.__type.name .. "\"")
    assert(copy.isValid)
    assert(copy ~= entity)

    testComponents(copy)

    testScripts(copy)

    testHierarchy(copy)

    copy:destroy()

    assert(copy)
    assert(not copy.isValid)
end

function Test:onInit()
    tick_count = 0

    if not testCalled then
        testCalled = true
        testECS(self.owner)
        self.owner:addScript("scripts.mathTest")
    end

    if self.max_tick ~= 0 then
        print("Entity " .. self.owner .. " already has a lifespan of " .. self.max_tick .. " frames...")
    else
        self.max_tick = math.random(1, 100)
        print("Entity " .. self.owner .. " now has " .. self.max_tick .. " frames to live. Enjoy!")
    end
end

function Test:onStart()
    print("Entity " .. self.owner .. ", reporting for duty!")
end

function Test:onFixedUpdate(deltaTime)
    fixed_tick_count = fixed_tick_count + 1
    fixed_time = fixed_time + deltaTime
end

function Test:onUpdate(deltaTime)
    tick_count = tick_count + 1
    time = time + deltaTime

    if tick_count == self.max_tick then
        local avgFrameTime = time / tick_count
        print("It's entity " .. self.owner .. "'s last tick!!! " ..
                "Average frame time: " .. avgFrameTime .. "s (" .. round(1 / avgFrameTime) .. "fps)")

        for i = 1, #req do
            print("[require " .. i .. "] " .. req[i]:someFunc() .. " | Passed instance count: " .. req[i].instances)
        end

        self.owner:destroy()
    elseif tick_count == self.max_tick + 1 then
        assert(false, "Not sur I want to give the user access to that...")
    end
end

function Test:onStop()
    print("Entity " .. self.owner .. " ready to stop")
end

function Test:onDestroy()
    local avgFrameTime = time / tick_count
    print("Destroyed test script of entity " .. self.owner .. " after " .. tick_count .. " ticks (" .. time .. "s) & " ..
            fixed_tick_count .. " fixed ticks (" .. fixed_time .. "s) | " ..
            "Average frame time: " .. avgFrameTime .. "s (" .. round(1 / avgFrameTime) .. "fps)")
end

return Test