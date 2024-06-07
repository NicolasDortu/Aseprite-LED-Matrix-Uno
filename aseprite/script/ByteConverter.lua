---@diagnostic disable: undefined-global, need-check-nil
local sprite = app.activeSprite
if not sprite then
  return app.alert("No sprite is active")
end

local spriteFilename = sprite.filename
local spriteDir = spriteFilename:match("(.*/)")
if not spriteDir then
  spriteDir = spriteFilename:match("(.*\\)")
end

local function toBinaryString(n)
  local t = {}
  for i = 7, 0, -1 do
    t[#t + 1] = (math.floor(n / 2 ^ i) % 2 == 1) and '1' or '0'
  end
  return table.concat(t)
end

local outputFile = io.open(spriteDir .. "frames.txt", "w")
local binaryStrings = {}

for frameIndex, frame in ipairs(sprite.frames) do
  local cel = sprite.layers[1]:cel(frame)
  if cel then
    for y = 0, 7 do   -- Pixel rows
      local byte = 0
      for x = 0, 7 do -- Pixel columns
        local pixelValue = app.pixelColor.rgbaR(cel.image:getPixel(x, y))
        if pixelValue > 0 then
          byte = byte | (1 << (7 - x))
        end
      end
      table.insert(binaryStrings, toBinaryString(byte))
    end
  else
    -- If there is no cel for the frame, write an empty frame
    for y = 0, 7 do
      table.insert(binaryStrings, "00000000")
    end
  end
end

outputFile:write(table.concat(binaryStrings, " "))
outputFile:close()
app.alert("Frames exported successfully to " .. spriteDir .. "frames.txt")
