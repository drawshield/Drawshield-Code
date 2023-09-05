# This is a sample Python script.

# Press Shift+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.
import sys

class Token:
    text = ''           # this is original set of characters from the blazon
    match = ''          # this is a length limited, lowercase version of the above for use in comparisons
    startPos = None     # this is a string in the form line:character giving the position of the START of the token
    endPos = None       # as above for the end, if null, just add the length of the tex
    category = None     # if not null this is the category to assign to the token, as determined by the tokeniser

    def __str__(self):
        print(self.text + " (" + self.match + ") from " + self.startPos + " to " + self.endPos + " Cat: " + self.category)

def tokenise(blazon):
    line_number = 1
    character_position = 1
    tokens = []

    i = 1   # counter through the blazon characters
    # various flags for states we can be in
    line_comment = False
    comment = False
    block_comment = False
    tilde_comment = False
    in_string = False
    include_all = False
    any_end_quote = False

    # temporary storage
    part_word = ''
    look_behind = ''
    word_start = 1
    end = len(blazon)

    while (i < end):
        # get the current characters to work on
        look_ahead = ''
        if (i + 1 < end):
            look_ahead = blazon[i+1]
        current_character = blazon[i]
        # deal with comment states
        if (block_comment):     # ignore everything to a matching close comment
            if (current_character == '*' and look_ahead == '/'):
                block_comment = False
                character_position += 1
                i += 1
            i += 1
            character_position += 1
            continue
        if (line_comment):
            if (current_character == "\n"):
                line_number += 1
                character_position = 0
                line_comment = False
            i += 1
            continue

        # Now we deal with specific character types (big switch statement)
        # Whitespace
        if (current_character == "\n" or
            current_character == " " or
            current_character == "\t" or
            current_character == "\r"):
            if (include_all):
                part_word += ' '
            elif (part_word != ""):
                if (not(comment)):
                    tokens.append(Token(part_word, f'{line_number}:{word_start}'))
                part_word = ''
                word_start = character_position + 1
            else:
                word_start = character_position + 1
            if (current_character == "\n"):
                line_number += 1
                character_position = 0
                word_start = 1
        elif (current_character == "{"):
            if (include_all):
                part_word += current_character
            else:
                if (part_word != ''):
                    if (not(comment)):
                        tokens.append(Token(part_word, f'{line_number}:{word_start}'))
                    part_word += "{"
                    include_all = True


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    for line in sys.stdin:
        if 'exit' == line.rstrip():
            break;
        tokenise(line)

