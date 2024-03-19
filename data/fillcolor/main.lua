-- pal(0,140,1)
-- rad = 0
function _init()
  pal(0)
end

function _draw()
  -- fillrect(0,0,50,50,7)
end

function _update()
  cls(1)
  fillp(14,12)--2色のストライプパタンのみ

  ox = 1
  oy = 1
  ovalfill(20*ox, 20*oy, 20, 20, 8,10)--カラー２つで2色
  oval(20*ox, 20*oy, 20, 20, 7)

  ox = 2
  ovalfill(20*ox, 20*oy, 20, 20, 2,12)--カラー２つで2色
  oval(20*ox, 20*oy, 20, 20, 7)

  ox = 3
  ovalfill(20*ox, 20*oy, 20, 20, 9)--カラー1つで1色
  oval(20*ox, 20*oy, 20, 20, 7)

  ox = 4
  fillp(3,11,11,11)--カラー設定をこっちでして
  ovalfill(20*ox, 20*oy, 20, 20)--カラーなし
  oval(20*ox, 20*oy, 20, 20, 7)

  ox = 5
  fillp(3,3,11,11)--カラー設定をこっちでして
  ovalfill(20*ox, 20*oy, 20, 20)--カラーなし
  oval(20*ox, 20*oy, 20, 20, 7)

  ox = 6
  fillp(11,3)--カラー設定をこっちでして
  ovalfill(20*ox, 20*oy, 20, 20)--カラーなし
  oval(20*ox, 20*oy, 20, 20, 7)

  ox = 7
  fillp(11,3,3,3)--カラー設定をこっちでして
  ovalfill(20*ox, 20*oy, 20, 20)--カラーなし
  oval(20*ox, 20*oy, 20, 20, 7)

  ox = 8
  ovalfill(20*ox, 20*oy, 20, 20, 8)--カラー1つで1色
  oval(20*ox, 20*oy, 20, 20, 10)

----------------------
    -- ox = 0
    -- oy = 4
    -- rectfill(20*ox, 20*oy, 40, 40, 9)--カラー1つで1色
    -- rect(20*ox, 20*oy, 40, 40, 7)

    -- ox = 1
    -- rectfill(20*ox, 20*oy, 40, 40, 3,12)--カラー2つで2色
    -- rect(20*ox, 20*oy, 40, 40, 7)

    -- ox = 2
    -- fillp(11,3,3,3)--カラー設定をこっちでして
    -- rectfill(20*ox, 20*oy, 40, 40)--カラーなし
    -- rect(20*ox, 20*oy, 40, 40, 7)

    -- ox = 3
    -- fillp(11,3)--カラー4色設定をこっちでして
    -- rectfill(20*ox, 20*oy, 40, 40)--カラーなし
    -- rect(20*ox, 20*oy, 40, 40, 7)

    -- ox = 4
    -- fillp(11,11,3,3)--カラー4色設定をこっちでして
    -- rectfill(20*ox, 20*oy, 40, 40)--カラーなし
    -- rect(20*ox, 20*oy, 40, 40, 7)

    -- ox = 5
    -- fillp(11,11,11,3)--カラー4色設定をこっちでして
    -- rectfill(20*ox, 20*oy, 40, 40)--カラーなし
    -- rect(20*ox, 20*oy, 40, 40, 7)

    -- ox = 6
    -- fillp(11,3,8,10)--カラー4色設定をこっちでして
    -- rectfill(20*ox, 20*oy, 40, 40)--カラーなし
    -- rect(20*ox, 20*oy, 40, 40, 7)

----------------------
    ox = 1
    oy = 3
    ovalfill(20*ox, 20*oy, 20, 20, 8,10)--カラー２つで2色
    oval(20*ox, 20*oy, 20, 20, 7)

    ox = 2
    ovalfill(20*ox, 20*oy, 20, 20, 2,12)--カラー２つで2色
    oval(20*ox, 20*oy, 20, 20, 7)

    ox = 3
    ovalfill(20*ox, 20*oy, 20, 20, 9)--カラー1つで1色
    oval(20*ox, 20*oy, 20, 20, 7)

    ox = 4
    fillp(3,11)--カラー設定をこっちでして
    ovalfill(20*ox, 20*oy, 20, 20)--カラーなし
    oval(20*ox, 20*oy, 20, 20, 7)

    ox = 5
    ovalfill(20*ox, 20*oy, 20, 20, 8)--カラー1つで1色
    oval(20*ox, 20*oy, 20, 20, 10)


    ox = 6
    -- fillp(3,3,11,11)--カラー設定をこっちでして
    fillp(11,3,8,10)
    ovalfill(20*ox, 20*oy, 20, 40)--カラーなし
    -- oval(20*ox, 20*oy, 20, 20, 7)


end
