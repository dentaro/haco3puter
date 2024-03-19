M_IDLE, M_MOVE, M_ANIM = 0, 1, 2
local mode, moveDir = M_IDLE, 0
local stage = {}

function _init()
    stage = {}
    for x = 1, 4 do
        stage[x] = {}
    end
    stage[1][1] = { value = 2 }
end

function addBlock()
    local emptyCells = {}
    for x = 1, 4 do
        for y = 1, 4 do
            if not stage[x][y] then
                table.insert(emptyCells, { x, y })
            end
        end
    end
    if #emptyCells == 0 then return false end
    local pos = emptyCells[flr(rnd(#emptyCells)) + 1]
    local newValue = (rnd(1) < 0.9) and 2 or 4  -- 90% の確率で2, 10% の確率で4
    stage[pos[1]][pos[2]] = { value = newValue }
    return true
end

function _move(x, y, px, py)
    if not stage[x][y] then return false end

    local nx, ny = x + px, y + py
    if not stage[nx][ny] then
        stage[nx][ny] = stage[x][y]
        stage[x][y] = nil
        return true
    elseif stage[x][y].value == stage[nx][ny].value then
        stage[nx][ny].value = stage[nx][ny].value * 2
        stage[x][y] = nil
        return true
    end

    return false
end

function move(dir)
    local moved = false
    if dir == 1 then
        for x = 2, 4 do
            for y = 1, 4 do
                moved = _move(x, y, -1, 0) or moved
            end
        end
    elseif dir == 2 then
        for x = 3, 1, -1 do
            for y = 1, 4 do
                moved = _move(x, y, 1, 0) or moved
            end
        end
    elseif dir == 3 then
        for x = 1, 4 do
            for y = 2, 4 do
                moved = _move(x, y, 0, -1) or moved
            end
        end
    elseif dir == 4 then
        for x = 1, 4 do
            for y = 3, 1, -1 do
                moved = _move(x, y, 0, 1) or moved
            end
        end
    end

    return moved
end

function input()

    if btn(6) >= 2 then--3
    appmode(appinfo(0), 1)--エディットモードで開く
    -- appmode("/2048/main.lua", 1)--エディットモードで開く
    end
      
    if mode == M_IDLE then
        for i = 1, 4 do
            if btn(i) == 2 then
                moveDir = i
                mode = M_MOVE
                break
            end
        end
    end
end

function _update()
    input()

    if mode == M_MOVE then
        local moved = move(moveDir)
        if moved then
            mode = M_ANIM
        else
            mode = M_IDLE
            addBlock()
        end
    end

    local animated = false
    -- animation
    for x = 1, 4 do
        for y = 1, 4 do
            local block = stage[x][y]
            if block then
                block.dx = mid((block.dx or 0) - 4, 0)
                block.dy = mid((block.dy or 0) - 4, 0)
                if block.dx ~= 0 or block.dy ~= 0 then
                    animated = true
                end
            end
        end
    end

    if mode == M_ANIM and not animated then
        mode = M_MOVE
    end
end

function drawBlock(x, y, block)
    drawrect(16 + x * 24 + (block.dx or 0), 16 + y * 24 + (block.dy or 0), 23, 23, block.value)
    text(flr(block.value), 16 + x * 24 + (block.dx or 0) + 2, 16 + y * 24 + (block.dy or 0) + 8, block.value)
end

function _draw()
    cls(15)
    drawrect(16, 16, 24*4, 24*4, 10)
    for x = 1, 4 do
        for y = 1, 4 do
            local block = stage[x][y]
            if block then
                drawBlock(x - 1, y - 1, block)
            end
        end
    end
    -- color(255, 255, 255)
    -- print("Mode: "..mode, 0, 0)
end
