# Favola

Favola is an authoring system for parserless IF, somewhere between Twine and traditional IF languages like Inform or TADS. My goal while creating Favola was to make an IF system that's as orthogonal as possible; to provide the greatest flexibility with the fewest built-in elements.

The core concept of Favola is that of focus. One or more game objects are in focus, and their associated actions can be performed either implicitly, or explicitly by clicking on an active text. Doing so may or may not change the current focus to a different object. Each performed action represents one game turn.

There are no built-in concepts of rooms, items, or compass directions. In this aspect Favola is like Twine, in that it concerns itself primarily with the text itself, rather than the simulation of the world. However, because Favola is an object-oriented programming language, simulation aspects can be implemented as well.



## Language Syntax

	#include "FileName"
	//comment
	
	//an object:
	Attributes Identifier "Name" : Parent1, Parent2, ...
	{
		//objects can have child objects
		ChildIdentifier : Parent	//the fully-qualified name will become Identifier.ChildIdentifier
		{
		}
	
		Attribute @Action	//define a new action
		{
	    	Hypertext
		}
	
		+Identifier			//add any previously defined object to this object
		-Identifier			//remove inherited action or object
	
		Property = "Properties are arbitrary key-value pairs"
		
		Function(parameter, ...)
		{
			//put code here
			return "Return value is optional"
		}
	}




## Objects

An object in the game can be anything, from the player character, a room, an item, an NPC or simply an abstract container for functions. From the viewpoint of the game engine, it's something that can be in or out of focus. If it's in focus, it provides actions that can be performed.

### Inheritance

An object can be derived from any number of existing objects by listing their comma-separated identifiers in the first line of the object definition, after a colon. For example `Auditorium : Location, Indoors` would create an object called `Auditorium` which inherits all attributes, child objects, actions, properties, and functions from two previously defined objects; `Location` and `Indoors`. If those two parent objects have some things in common, `Indoors` would override `Location`, as it's listed second.

### Object Attributes

Attributes are listed before the object identifier, separated by spaces. They can be used in expressions like Boolean properties. Some are read-only.

Qualifier | Meaning
----|----
Ubiquitous | Object is always in focus, unless hidden (e.g. inventory). 
Prominent | Object is in focus if its parent is in focus. 
Hidden | Object is hidden. 
Focused | (Read-only) The first focused object when game starts.<br />This attribute is set by the game engine as focus shifts. 

### Properties

Object properties are key-value pairs without any built-in meaning. The only exception is the `Name` property, which can be set by writing its value in double quotation marks after an object's identifier. For example, `Auditorium "School Auditorium"` sets the value of the `Auditorium.Name` property to `School Auditorium`.

### Focus

At least one game object is always in focus. The game also considers all of its parent objects to be in focus, albeit to a lesser extent; explicit actions of these objects are not shown in the action bar. Objects that have the attribute `Ubiquitous` are also considered to be fully in focus, as are `Prominent` child objects of the main focused object. Active text within already displayed hypertext remains active as long as the object whose action generated it or its child is still in focus. Focus is shifted from one object to another when the player activates hyperlinks.



Actions
----------

An action is defined by starting an identifier with an @ sign. Note that unlike objects and functions, action identifiers can consist of multiple words. Writing `@` without an identifier is a shortcut for creating an implicit action, i.e. one that is performed immediately when it becomes available. An example of an implicit action would be looking around when the player enters a new location. During gameplay, any non-implicit actions of the object(s) currently in focus will be shown along the bottom of the game window.

For example, if there's a `@Smell` action defined for the object currently in focus, a clickable "Smell" text will appear in the bottom action bar. You can use this mechanism to define actions you don't want to integrate into the hypertext itself. Instead of writing "In the vase, there's a bouquet of pretty flowers. You could try <u>smelling</u> them." you can simply write "In the vase, there's a bouquet of pretty flowers." and add a `@Smell flowers` action. This way the text reads more like a normal book rather than a CYOA.

### Action Attributes

Each action can have a number of attributes that influence how it is performed. They are written before the @ sign and are separated by spaces.

Attribute | Meaning
---|---
`Implicit` | Performed immediately when the object becomes focused 
`Hidden` | Not performed or shown in the action bar until revealed in a function 
`Perpetual` | (Implies Implicit) Performed on every turn while the object is in focus 

### Hypertext

Action hypertext can contain active elements enclosed in square brackets [].

| Format                       | Meaning                                                      |
| ---------------------------- | ------------------------------------------------------------ |
| `[expression]`               | Will be replaced with the result of the expression.          |
| `[expression->expression]`   | The result of the first expression will appear <u>underlined</u>.<br />When clicked, shifts focus to the object resulting from the second expression. |
| `[expression^]`              | Same as above, but shifts focus to the parent.               |
| `[random:item1|item2|...]`   | Will be replaced with a random item from the list.           |
| `[sequence:item1|item2|...]` | Will be replaced with one of the items sequentially.<br />Once the sequence is finished, the last item will be repeated. |
| `[loop:item1|item2|...]`     | Same as above, but with looping back to the first item instead. |
| `[chance N%:expression]`     | Has an N% chance of being replaced with the result of the expression. |

These can be nested to arbitrary depth. For example, to create an element which is either one of two different expressions or nothing, write:
`[random:[expression1]|[expression2]|]`

### Special Characters

| Escape Sequence       | Meaning                                                      |
| --------------------- | ------------------------------------------------------------ |
| \n                    | Forced line break (newline).                                 |
| \p                    | Forced paragraph break.<br />Paragraph breaks are normally inserted at the start of each turn. |
| \\>                   | Increase indent (until end of paragraph).                    |
| \\<                   | Decrease indent (implies forced line break).                 |
| \\any other character | That character, without any special meaning.<br />I.e. `\}` would insert a curly brace without closing the hypertext block. |



## Functions

Functions can be used in active elements of action hypertext, or called from other functions.

### Code Syntax

Code in functions has a very simple C-like syntax, with only a minimal set of keywords.

| Keyword(s)           | Function                     |
| -------------------- | ---------------------------- |
| if (cond) {} else {} | Conditional execution        |
| while (cond) {}      | Loop while condition is true |
| break                | Break out of a loop          |
| return exp           | Return from function         |
| variable = exp       | Assignment                   |
