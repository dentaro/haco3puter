x=80
y=64
tx=0
ty=0
ax=171--lcx(171)
ay=200--lcy(218)
wx=156--0
wy=187--36
px=0
py=0
wstat=1
function input()
  -- if btn(1) >= 1 then wx = wx-1 end
  -- if btn(2) >= 1 then wx = wx+1 end
  -- if btn(3) >= 1 then wy = wy-1 end
  -- if btn(4) >= 1 then wy = wy+1 end
  if btnp(1) then wx = wx-1 end
  if btnp(2) then wx = wx+1 end
  if btnp(3) then wy = wy-1 end
  if btnp(4) then wy = wy+1 end
end

function _init()--1回だけ
  pal(1)
end
function _update()
  input()
  wx = wx%256
  wy = wy%256
end
function _draw()
  rmap("/init/param/map/w.bin",wx,wy)
  spr8(64,x,y,1,1,0, 0,10)--主人公スプライト透明色指定のあとに色番号をつけるとふちどり

  -- HPMP=1
  --   win(wstat, 0,   0, 1, 2, 3, HPMP,"ああああ")
  --   win(wstat, 40,  0, 1, 2, 3, HPMP,"いいいい")
  --   win(wstat, 80,  0, 1, 2, 3, HPMP,"うううう")
  --   win(wstat, 120, 0, 1, 2, 3, HPMP,"ええええ")
  --   rectfill(56, 32, 48, 90, 0, "/world/png/enemy/3.png") --160,48が敵エリアの最大値
  --   win(wstat, 0,80,1, 4,18, 0,"むらびと")
end