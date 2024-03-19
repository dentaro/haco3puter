
x=12
y=12
z=12
prex = 12
prey = 12

radle = 0
s=1 -- スプライト番号
hmap = {
70,60,50,40,
60,60,40,30,
50,40,30,20,
40,30,20,10
}
rad = 0

v0 = 9       -- 初速度
time = 0     -- 時間
gravity= 0.9 -- 重力加速度
canJump = false
py = 0

--  addbtn(9,100,0,20,20,2)

function collision(x,y)
end

function _init()
 -- wini(32)
  -- music(0);
end

function input()
  -- x = tp(0)
  -- y = tp(1)

  if btn(1) >= 2 then --1
    prex = prex - 1
    --x = x - 1  -- 左に移動
  elseif btn(2) >= 2 then--2
    prex = prex + 1
    --x = x + 1  -- 右に移動
  end

  if btn(6) >= 2 then--3
    appmode(appinfo(0), 1)--エディットモードで開く
    -- appmode("/fake3d/main.lua", 1)--エディットモードで開く
  end

  if btn(3) >= 2 then--3
    prey = prey - 1
    --y = y - 1  -- 上に移動
  elseif btn(4) >= 2 then--4
    prey = prey + 1
    --y = y + 1  -- 下に移動
  end
  if btn(2) >= 2 then--8
    radle = radle - 1  -- 左に回転
  elseif btn(1) >= 2 then--7
    radle = radle + 1    -- 右に回転
  end

  if btn(3) >= 2 then--3
    canjump = false
  end
end

function _update()
  input()
end

function _draw()
  cls(0)
  -- drawpng("/img.png", 0, 0)
  unit = 10
  -- for ty = 0,3 do
  --   for tx = 0,3 do
  --     -- fillpoly(tx*unit*2, ty*unit, tx*unit*2+10, ty*unit+5, tx*unit*2+10, ty*unit+15, tx*unit*2, ty*unit+10, tx*4+ty)
  --     -- fillpoly(tx*unit*2+unit, ty*unit+5, tx*unit*2+unit+10, ty*unit, tx*unit*2+unit+10, ty*unit+10, tx*unit*2+unit, ty*unit+15, tx*4+ty)
  --     drawrect( tx*unit, ty*unit, unit,unit, ty*4+tx )
  --     drawbox( 80,75, tx*unit, ty*unit, 65-(ty*4+tx)*4, ty*4+tx )
  --   end
  -- end
  if rad>6.28 then
    rad = 0
  end

  rad = rad + 3.1415/180


  -- addbtn(9,abs(100*gcos(rad)),0)
  -- drawrect(abs(100*gcos(rad)),0,20,20,8)

  -- for j = 0,1 do
  --   for i = 0,1 do
  --     addbtn(10+j*2+i,abs(100*gcos(rad))+20*i,20+20*j)
  --     drawrect(abs(100*gcos(rad))+20*i,20+20*j,20,20,3)
  --   end
  -- end

  -- addbtn(11,abs(100*gcos(rad)),40,20,20,2)
  -- drawrect(abs(100*gcos(rad)),40,20,20,10)

  if prex<=0 then prex = 39 end
  if prex>=40 then prex = 1 end
  if prey<=0 then prey = 39 end
  if prey>=40 then prey = 1 end

  for ty = 0,3 do
      for tx = 0,3 do
    local h = hmap[ty*4+tx+1]
    drawbox( 80, 75, tx*unit, ty*unit, abs(h*cos(rad)), ty*4+tx )
    drawrect( tx*unit, ty*unit, unit,unit, ty*4+tx )
    end
  end

  prepos = drawboxp( x-4, y-4, abs(hmap[flr(prey/unit)*4+flr(prex/unit)+1]*cos(rad)) )

  if prepos.y > py then
    x = prex
    y = prey
  else 
    prex = x
    prey = y
  end

  pos = drawboxp( x-4, y-4, abs(hmap[flr(y/unit)*4+flr(x/unit)+1]*cos(rad)) )
  
  -- spr8(s+64, flr(pos.x), flr(pos.y) )
  --spr8(s+64,x, y, 1)--1はbox座標上にマップ

  if canjump == true then
    py = pos.y
    time = 0

  elseif canjump == false then

    py = 0.5*gravity*time*time - v0*time + pos.y

    if py > pos.y then
      py = pos.y
      canjump = true
      -- time = 0
    end
    time = time + 1
  end

  spr8(s+64, flr(pos.x), flr(py) )
  spr8(s+64, x, y)
  
end

