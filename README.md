# Favola

Favola is an authoring system for parserless IF, somewhere between Twine and traditional IF languages like Inform or TADS. My goal was to make an IF system that's as orthogonal as possible; to provide the greatest flexibility with the fewest built-in elements. It's also aimed more at programmers, who are familiar with languages like C++ or C#.

The core concept of Favola is that of focus. One or more game objects are in focus, and their associated actions can be performed either implicitly, or explicitly by clicking on an active text. Doing so may or may not change the current focus to a different object. Each performed action represents one game turn.

There are no built-in concepts of rooms, items, or compass directions. In this aspect Favola is like Twine, in that it concerns itself primarily with the text itself, rather than the simulation of the world. However, because Favola is an object-oriented programming language, simulation elements can be implemented as well.



## Language Syntax

	//this is a comment, directives start with a pound sign #
	
	#include "FileName"			//a game can have multiple source files
	#css "MyStyleSheet.css"		//there's one style sheet per game
	#markdown					//hypertext can be either HTML or Markdown
	
	//an object:
	Attributes Identifier "Name" : Parent1, Parent2, ...
	{
		//identifiers are *not* case sensitive
		//objects can have child objects
		ChildIdentifier : Parent	//the fully-qualified name will become Identifier.ChildIdentifier
		{
		}
	
		Attributes @Identifier "Name"	//define a new action
		{
	    	Hypertext
		}
	
		+Identifier			//add any previously defined object or action to this object
		-Identifier			//remove inherited action or object
	
		Property = "Properties are arbitrary key-value pairs"
		
		Function(parameter, ...)
		{
			//put C-like code here
			return "Return value is optional";
		}
	}




## Objects

An object in the game can be anything, from the player character, a room, an item, an NPC or simply an abstract container for functions. From the viewpoint of the game engine, it's something that can be in or out of focus. If it's in focus, it provides actions that can be performed.

### Inheritance

An object can be derived from any number of existing objects by listing their comma-separated identifiers in the first line of the object definition, after a colon. For example `Auditorium : Location, Indoors` would create an object called `Auditorium` which inherits all attributes, child objects, actions, properties, and functions from two previously defined objects; `Location` and `Indoors`. If those two parent objects have some things in common, `Indoors` would override `Location`, as it's listed second.

### Object Attributes

Attributes are listed before the object identifier, separated by spaces. They can be used in expressions like Boolean properties. Some are read-only.

Attribute | Meaning
----|----
`Ubiquitous` | Object is always in focus, unless hidden (e.g. inventory). 
`Prominent` | Object is in focus if its parent is in focus. 
`Hidden` | Object is hidden. 
`Focused` | (Read-only) The first focused object when game starts.<br />This attribute is set by the game engine as focus shifts. 

### Properties

Object properties are key-value pairs without any built-in meaning. The only exception is the `Name` property, which can be set by writing its value in double quotation marks after an object's identifier. For example, `Auditorium "School Auditorium"` sets the value of the `Auditorium.Name` property to `School Auditorium`.

### Focus

At least one game object is always in focus. The game also considers all of its parent objects to be in focus, albeit to a lesser extent; explicit actions of these objects are not shown in the action bar. Objects that have the attribute `Ubiquitous` are also considered to be fully in focus, as are `Prominent` child objects of the main focused object. Active text within already displayed hypertext remains active as long as the object whose action generated it or its child is still in focus. Focus is shifted from one object to another when the player activates hyperlinks.



Actions
----------

An action is defined by starting an identifier with an @ sign. Giving a name to the action makes it explicit by default (unless prefixed with the `Implicit` attribute). During gameplay, any named actions of the object(s) currently in focus will be shown along the bottom of the game window. Omitting a name makes the action implicit, which means it will be performed immediately when the object that owns it becomes focused. An example of an implicit action would be looking around when the player enters a new location. An action identifier is required only in order to trigger the action from hypertext, otherwise it can be omitted.

For example, if there's a `@"Smell"` action defined for the object currently in focus, a clickable "Smell" text will appear in the bottom action bar. You can use this mechanism to define actions you don't want to integrate into the hypertext itself. Instead of writing "In the vase, there's a bouquet of flowers. You could try <u>smelling</u> them." you can simply write "In the vase, there's a bouquet of flowers." and add a `@"Smell flowers"` action. This way the text reads more like a normal book rather than a CYOA.

### Action Attributes

Each action can have a number of attributes that influence how it is performed. They are written before the @ sign and are separated by spaces.

Attribute | Meaning
---|---
`Implicit` | Performed immediately when the object becomes focused,<br />regardless if the action is named or not 
`Hidden` | Not performed or shown in the action bar until revealed in a function 
`Perpetual` | (Implies Implicit) Performed on every turn while the object is in focus 

### Hypertext

Action hypertext is written in HTML or Markdown, and can be styled using CSS. To switch between HTML and Markdown, use the `#html` and `#markdown` directives, which take effect until the next directive or the end of the current source file. The style sheet is common for the whole game, and is set using the `#css "MyStyles.css"` directive. If there are multiple such directives, the last one encountered takes effect.

Hypertext can also contain active elements enclosed in square brackets `[]`. These evaluate expressions, create active (clickable) text, and allow you to easily introduce a bit of randomness into the game.

| Format                       | Meaning                                                      |
| ---------------------------- | ------------------------------------------------------------ |
| `[expression]`               | Will be replaced with the result of the expression.          |
| `[expression@expression]`   | The result of the first expression will appear <u>underlined</u>.<br />When clicked, performs the action resulting from the second expression.<br />If the first expression is omitted or results in an empty string, the action will be performed immediately. |
| `[expression->expression]`   | The result of the first expression will appear <u>underlined</u>.<br />When clicked, shifts focus to the object resulting from the second expression.<br />If the first expression is omitted or results in an empty string, the focus will shift to the new object immediately. |
| `[expression^]`              | Same as above, but shifts focus to the parent object.<br />If the first expression is omitted or results in an empty string, the focus will shift to the parent object immediately. |
| `[random:item1\|item2\|...]`   | Will be replaced with a random item from the list.           |
| `[sequence:item1\|item2\|...]` | Will be replaced with one of the items sequentially.<br />Once the sequence is finished, the last item will be repeated. |
| `[loop:item1\|item2\|...]`     | Same as above, but with looping back to the first item instead. |
| `[chance N%:expression]`     | Has an N% chance of being replaced with the result of the expression. |

These can be nested to arbitrary depth. For example, to create an element which is either one of two different expressions or nothing, write:
`[random:[expression1]|[expression2]|]`

Special characters like `}`, `[`, `]` or `|` can be escaped using `\` backslash. Use `\\` to output a single backslash.



## Functions

Functions can be used in active elements of action hypertext, or called from other functions.

### Code Syntax

Code in functions has a very simple C-like syntax, with only a minimal set of keywords.

| Keyword(s)           | Function                     |
| -------------------- | ---------------------------- |
| `if (cond) {} else {}` | Conditional execution        |
| `while (cond) {}`      | Loop while condition is true |
| `break`                | Break out of a loop          |
| `return exp`           | Return from function         |
| `variable = exp`       | Assignment                   |



## Multimedia & Custom Fonts

Favola games can contain static images, music and sound effects. They can also use custom TrueType (.ttf) fonts.

Images are inserted directly into the hypertext using `<img />` elements. Their URLs should start with `game://` which tells the engine to look for them in the same folder as the source file(s).

Music & sound: TBD.

Fonts: TBD.



## Packaged Games

TBD
