import os
import re
import ConfigParser

debug = False
if debug:
  console.clear()
  console.show()

styleEOL = 100
styleRangeFrom = 101

def main():
   editor.setProperty('FWVisualizerType', promptFileType())

   if initRegexedRecords() == 0:
      return

   initStyles()
   initScintilla()
   editor.setCaretLineFrame(2)

   editor.callbackSync(styleVisualizer, [SCINTILLANOTIFICATION.STYLENEEDED])
   notepad.callback(bufferActivated, [NOTIFICATION.BUFFERACTIVATED])

   notepad.activateBufferID(notepad.getCurrentBufferID())


def initStyles():
   global styleEOLValue
   global styleRangeValues

   configStyles = ConfigParser.ConfigParser()
   configStyles.read(currDir() + '/' + configViz.get(editor.getProperty('FWVisualizerType'), 'FileTheme') + '.ini')

   styleEOLValue = [ tuple(toIntList( configStyles.get('EOL', 'Back') )),
                     tuple(toIntList( configStyles.get('EOL', 'Fore') )),
                     (configStyles.get('EOL', 'Bold') == 'Y'),
                     (configStyles.get('EOL', 'Italics') == 'Y') ]

   styleRangeValues = []
   for i in range( int(configStyles.get('Styles', 'Count')) ):
      prefix = 'C' + ('0' + str(i))[-2:] + '_'
      styleRangeValues.append (( tuple(toIntList( configStyles.get('Styles', prefix + 'Back') )),
                                 tuple(toIntList( configStyles.get('Styles', prefix + 'Fore') )),
                                 configStyles.get('Styles', prefix + 'Bold') == 'Y',
                                 configStyles.get('Styles', prefix + 'Italics') == 'Y' ))


def promptFileType():
   global configViz

   configViz = ConfigParser.ConfigParser()
   configViz.read(currDir() + '/Visualizer.ini')

   ftList = configViz.get('Base', 'FileTypes').split(',')

   ftPrompt = 'Please note the numeric code for the desired File Type\n\n'
   for key, val in enumerate(ftList):
      ftPrompt = ftPrompt + str(key) + ': ' + configViz.get(val, 'FileLabel') + '\n'

   notepad.messageBox(ftPrompt, 'Available File Types')

   configPrefs = ConfigParser.ConfigParser()
   configPrefs.read(currDir() + '/VisualizerPrefs.ini')
   try:
      ftLast = configPrefs.get('Defaults', 'LastFileType')
   except:
      ftLast = '0'

   ftCode = promptValue(sInfoText = 'Specify File Type Code (between 0 and ' + str(len(ftList) - 1) + '):',
                           sTitleText = 'Specify File Type Code',
                           sDefaultVal = ftLast,
                           iMinVal = 0,
                           iMaxVal = len(ftList),
                           sRangeError = 'Max File Type Code is ' + str(len(ftList)),
                           bRepeatPrompt = True)
   if ftCode == None:
      return ''

   configPrefs.set('Defaults', 'LastFileType', str(ftCode))
   with open(currDir() + '/VisualizerPrefs.ini', 'wb') as configSavePrefs:
       configPrefs.write(configSavePrefs)

   return ftList[ftCode]

def initRegexedRecords():
   global regexedRecords

   ftSelected = editor.getProperty('FWVisualizerType')
   if ftSelected == '':
      return 0

   # Init Record Types
   regexedRecords = {}
   rtList = configViz.get(ftSelected, 'RecordTypes').split(',')

   for recType in rtList:
      rtwList = toIntList( configViz.get(ftSelected, recType + '_FieldWidths') )
      rtMatch = configViz.get(ftSelected, recType + '_Marker')

      regexedRecords[rtMatch] = (re.compile(rtMatch), rtwList)

   return len(regexedRecords)

def currDir():
   return os.path.dirname(os.path.abspath(__file__))


def toIntList(val):
   arr = val.split(',')
   for key, val in enumerate(arr):
      arr[key] = int(val)
   return arr


def promptValue(sInfoText, sTitleText, sDefaultVal, iMinVal, iMaxVal, sRangeError, bRepeatPrompt):
   while True:
      sNewVal = notepad.prompt(sInfoText, sTitleText, sDefaultVal)
      if sNewVal == None:
         return None

      try:
         iNewVal = int(sNewVal)
         if iMinVal <= iNewVal <= iMaxVal:
            return iNewVal
         else:
            raise
      except:
         notepad.messageBox(sRangeError + '.\n\nYou specified: ' + sNewVal +
                              '\n\nPlease specify a number between ' + str(iMinVal) + ' and ' + str(iMaxVal) + '.',
                              'Specified value is out of range')
         if not bRepeatPrompt:
            return None


def initScintilla():
   if editor.getLexer() == LEXER.CONTAINER:
      return

   # Set EOL Style
   editor.styleSetBack(styleEOL, styleEOLValue[0])
   editor.styleSetFore(styleEOL, styleEOLValue[1])
   editor.styleSetBold(styleEOL, styleEOLValue[2])
   editor.styleSetItalic(styleEOL, styleEOLValue[3])

   # Set Field Styles
   for i, val in enumerate(styleRangeValues):
      editor.styleSetBack(styleRangeFrom + i, val[0])
      editor.styleSetFore(styleRangeFrom + i, val[1])
      editor.styleSetBold(styleRangeFrom + i, val[2])
      editor.styleSetItalic(styleRangeFrom + i, val[3])

   editor.setLexer(LEXER.CONTAINER)
   editor.startStyling(0, 0)


def bufferActivated(args):
   if initRegexedRecords() == 0:
      return

   initScintilla()


def styleVisualizer(args):
   styleRangeLen = len(styleRangeValues)
   eolMarker = configViz.get(editor.getProperty('FWVisualizerType'), 'RecordTerminator')
   eolMarkerLen = len(eolMarker)

   # Begin Styling
   startLine = editor.lineFromPosition(editor.getEndStyled())
   endLine = editor.lineFromPosition(args['position'])

   currentLine = startLine
   newRec = True

   while (currentLine < endLine):
      lineText = editor.getLine(currentLine)
      endPos = editor.getLineEndPosition(currentLine)

      if newRec:
         recStartPos = editor.positionFromLine(currentLine)
         recStartText = lineText

      currentLine += 1

      if ( (eolMarkerLen == 0) or (lineText.rstrip()[-eolMarkerLen:] == eolMarker) ):
         newRec = True
         eolMarkerPos = endPos - eolMarkerLen
      else:
         if (currentLine < endLine):
            newRec = False
            continue
         else:
            eolMarkerPos = endPos

      currentPos = recStartPos

      if debug:
         console.write('< Current Line = ' + str(currentLine) + ' >\n')

      colorOffset = 0
      lineFields = []
      for key, val in regexedRecords.items():
         if val[0].match(recStartText):
            lineFields = val[1]
            break

         colorOffset += 5

      if debug:
         console.write('<' + str(len(lineFields)) + '>   ' + str(lineFields) + '\n')

      for i, fLen in enumerate(lineFields):
         editor.startStyling(currentPos, 0)
         unstyledLen = eolMarkerPos - currentPos
         currentPos += fLen

         if fLen < unstyledLen:
            editor.setStyling(fLen, styleRangeFrom + ((i + colorOffset) % styleRangeLen) )
         else:
            editor.setStyling(unstyledLen, styleRangeFrom + ((i + colorOffset) % styleRangeLen) )
            editor.startStyling(eolMarkerPos, 0)
            editor.setStyling(eolMarkerLen, styleEOL)
            break


main ()
