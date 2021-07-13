# Common Tasks

This document describes how to perform tasks in *Favola*, which are common to most types of IF.

## The Basics

Arguably the most basic of player activities in traditional IF are travel from one location to the next, and manipulating items. The parserless IF system *Twine* has a concept of passages and of moving between them, even if they don't necessarily have to represent locations. Item handling in turn can be implemented using progressively revealed text or separate passages.

*Favola* is parserless as well, but instead of *Twine*'s passages, it uses the concepts of *objects* and *focus*. The narrator of the game is *focused* on a particular *object*, which, like a *passage* in *Twine*, can be anything from a location to a simple item.  Each *object* has implicit and/or explicit *actions* (such as "look at" or "pick up") and these generate hypertext. Links in the hypertext may then shift the narrator's focus to a different object.

This is all a bit abstract, so let's look at some examples:

	Darkness
	{
		@{You're floating in darkness, and there's really nothing to do.<br/>*The End*}
	}

This is a valid *Favola* game, albeit a very short one.

On the first line, we've created an object with the identifier `Darkness` (identifier means engine-facing name, as opposed to player-facing).

The following `{ }` block delimits the contents of the object.

The `@` sign creates an *action*. Because our action doesn't have a name, it is automatically considered to be *implicit*, which means it will be performed as soon as the object that owns it (`Darkness` in this case) comes into focus. Since *Favola* automatically focuses the first object it encounters, our action will be performed as soon as the game starts.

The action's hypertext (in Markdown format in this case) is in the `{ }` block after the `@` sign.

Running this game will produce the following output:

​	You're floating in darkness, and there's really nothing to do.
​	*The End*

Not very exciting. Let's change the example a bit:

	Darkness
	{
		@{You're floating in darkness, and there doesn't seem to be much to do.}
		@"Fiat lux!" {You say "Fiat lux!" but nothing happens. Well, it was worth a try.}
	}

We've added a second, *explicit* action, with the player-facing name "Fiat lux!". This name will appear in the action bar at the bottom of the screen, whenever the `Darkness` object is focused. Clicking the action will add its hypertext to the focused object's current text, (if any) and also set the action's `Hidden` property to true. This will hide it from the action bar. The default assumption is that the player won't be performing the same action over and over.

If you don't want to place the action link on the action bar, but within the text itself, here's how you would do it:

	Darkness
	{
		@{You're floating in darkness, and there doesn't seem to be much to do.
	 	  A phrase you've read in some book suddenly springs to your mind: "[Fiat lux!@Light]."}
		@Light {You say "Fiat lux!" but nothing happens. Well, it was worth a try.}
	}

Square brackets within a hypertext mark active elements. In the example above, the active element `[Fiat lux!@Light]` creates a hyperlink with the text <u>Fiat lux!</u>, which will trigger the action with the identifier `Light`, defined further down. When the user clicks the hyperlink, the action's hypertext will be added to the focused object's text, and the action's `Hidden` property will be set to true, which will transform the hyperlink into plain text.

