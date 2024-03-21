cl = 4
-- sound=-1
function get_map_sprn(sprx,spry)
  local celx=flr(sprx/8)
  local cely=flr(spry/8)
  local celc=mget(celx,cely)
  return celc
end

function get_map_flag(sprx,spry)
  local celx=flr(sprx/8)
  local cely=flr(spry/8)
  local celc=mget(celx,cely)
  -- return celc
  return fget(celc,0)
end

function collision(x,y)
  return get_map_flag(x-cl,y+2-cl) or get_map_flag(x+6-cl,y+2-cl) or get_map_flag(x-cl,y+7-cl) or get_map_flag(x+6-cl,y+7-cl)
end
cno = 1
sprno = 0
presprno =0
x=80
y=64
wx=163--0
wy=211--36
prewx = 0
prewy = 0
v = 1
lefttop=0
righttop=0
leftbottom=0
rightbottom=0

s=1 -- スプライト番号
d=1 -- 方向を示す                                                                                  
ipf=8 -- アニメーション1フレームについての時間(1ipf = 1/30秒)
nf=2 -- アニメーションするフレーム数(足踏みは2フレーム)
t=0
unit = 16

-- last_sound=-1

function setup()
  cls(0)
  tool(1);--ツール1はカラーパレットツールの表示
  
end

function _init()--1回だけ
    setup()
end


function input()
  if btn(6) >= 2 then
    -- appmode(appinfo(0), 1)--エディットモードで開く
  end
  
  local prex = x
  local prey = y
  local pressed=false

  if btnp(1) then
    sprno=(sprno-1)
    if sprno <0 then
      sprno=sprno+128
    end
  end
  if btnp(2) then
    sprno=(sprno+1)
    if sprno >128 then
      sprno=sprno-128
    end
  end
  if btnp(3) then
    sprno=sprno-8
    if sprno <0 then
      sprno=sprno+128
    end
  end
  if btnp(4) then
    sprno=sprno+8
    if sprno <0 then
      sprno=sprno+128
    end
  end

  if (btn(25)>=2) then cno = 0 end
  if (btn(26)>=2) then cno = 1 end
  if (btn(27)>=2) then cno = 2 end
  if (btn(28)>=2) then cno = 3 end
  if (btn(29)>=2) then cno = 4 end
  if (btn(30)>=2) then cno = 5 end
  if (btn(31)>=2) then cno = 6 end
  if (btn(32)>=2) then cno = 7 end

  if (btn(33)>=2) then cno = 8 end
  if (btn(34)>=2) then cno = 9 end
  if (btn(35)>=2) then cno = 10 end
  if (btn(36)>=2) then cno = 11 end
  if (btn(37)>=2) then cno = 12 end
  if (btn(38)>=2) then cno = 13 end
  if (btn(39)>=2) then cno = 14 end
  if (btn(40)>=2) then cno = 15 end

  if pressed then
   s=d+flr((t%(nf*ipf))/ipf+1)*8 --8個先が次のコマ
  else
   s=d
  end

  if collision(x,y) == true then
    -- sound=8
    x=prex
    y=prey
  end

 end

function _update()
  input()
end

function _draw()
  fillrect(128+3,112,16,16,cno)
  drawrect(128+3,112,16,16,8)

  if wx~=presprno or wy~=presprno then
    cls(0)
    drawrect(0,0,128,128,0)
    spr8(sprno%128,unit*2+40,0+72, 16,16)--キャラ拡大プレビュー
    drawrect(0,0,128,128,8)
    
    spr8(sprno%128,128+13,8, 2,2)--キャラ2倍プレビュー

    for j= 0 , 1 do
      for i = 0 , 1 do
        spr8(sprno%128,  136+i*8, 24+j*8)--8*4キャラ実寸プレビュー
      end
    end 

    for j= 0 , 7 do
      for i = 0 , 2 do
        spr8((sprno+j*8+i)%128, 136+i*8, 44+j*8)--16キャラ実寸プレビュー
      end
    end 
    drawrect(132,40,24,64,8)
    
  end

  

  
  

  -- print(info(1),120,96)--info0でボタンidを取得して表示
  print(tp(0),120,104)--x座標を表示
  print(tp(1),120,112)--y座標を表示
  presprno = sprno

  -- prewx = wx
  -- prewy = wy
end

