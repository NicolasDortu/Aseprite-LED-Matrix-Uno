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

outputFile:write("byte frames[][8] = {\n")

for frameIndex, frame in ipairs(sprite.frames) do
  outputFile:write("  {\n")
  local cel = sprite.layers[1]:cel(frame)
  if cel then
    for y = 1, 8 do                                                                 -- Modification: start y from 1 to 8
      local byte = 0
      for x = 1, 8 do                                                               -- Modification: start x from 1 to 8
        if x <= cel.image.width and y <= cel.image.height then                      -- Adjust bounds check
          local pixelValue = app.pixelColor.rgbaR(cel.image:getPixel(x - 1, y - 1)) -- Modification: adjust pixel coordinates
          if pixelValue > 0 then
            byte = byte | (1 << (8 - x))                                            -- Modification: shift the bit to the correct position
          end
        end
      end
      outputFile:write(string.format("    B%s,\n", toBinaryString(byte)))
    end
  else
    -- If there is no cel for the frame, write an empty frame
    for y = 1, 8 do
      outputFile:write("    B00000000,\n")
    end
  end
  outputFile:write("  },\n")
end

outputFile:write("};\n")
outputFile:close()
app.alert("Frames exported successfully to " .. spriteDir .. "frames.txt")
