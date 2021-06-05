# The Doppelkopf Interface (DKI)

[TOC]

## Principles
 * The engine must only calculate when receiving a request
 * The engine must always be able to process commands (even whilst calculating)
 * No two requests should be unanswered at the same time
 * (Debug / Error / Warning) messages should be passed from the engine to the UI via the `log` command
 * The engine may (and should) keep track of state-information

## Serialization
The exchange between the engine and the user interface is a series of **commands**. Each command takes the form `<command> <arguments>` where the command is one of the strings listed [below](#Commands). The arguments are a (possibly empty) series of strings where the elements are separated by at least one whitespace. The interpretation of the arguments is based on their position and the specified command. The end of the argument-list is denoted by a linebreak (CRLF and LF must both be handled equally by the engine and user interface).

It follows a list of argument-types and their serialization.

Type    | Serialization
--------|--------------
Boolean | Single character: `0` for `false` and `1` for `true`.
String  | A series of characters. A string may contain whitespaces and may be wrapped in quotes.
Card    | The type: `<C/S/H/D>` (Clubs / Spades / Hearts / Diamonds) directly followed by the value: `<A/K/Q/J/10/9>` (Ace / King / Queen / Jack / Ten / Nine).<br/>*E.g.: `H10` is the **ten of hearts** and `CQ` is the **queen of clubs***
Arrays  | An array is serialized as the values concatenated using `,` (no spaces).<br/>*E.g.: `H10,CQ` denotes the Array **{ten of hearts, queen of clubs}***
State   | *TODO*

## Commands
>todo explain syntax

### Misc
* **`log <level> <message>` \[E&rarr;U\]** &ndash; Tells the user interface to take note of the provided message<br/>
    *TODO: long description*

### Setup
* **`getsupportedrules` \[U&rarr;E\]** &ndash; Checks what rules are supported by the engine<br/>
    *TODO: long description*

* **`supportedrules <str>...` \[E&rarr;U\]** &ndash; Used by the engine to tell the user interface what rules it supports<br/>
    *TODO: long description*

* **`setrules <key[=value]>...` \[U&rarr;E\]** &ndash; Tell the engine with which rules to play<br/>
    *TODO: long description*

* **`ready?` \[U&rarr;E\]** &ndash; Used by the user interface to check if the engine is ready<br/>
    *TODO: long description*

* **`ready` \[E&rarr;U\]** &ndash; Used by the engine to tell the user interface that it is ready<br/>
    *TODO: long description*

* **`setstate` \[U&rarr;E\]** &ndash; Used by the user interface to tell the engine the cards on its hand and its id<br/>
    *TODO: long description*

### In Game
#### Reservations
* **`reservations?` \[U&rarr;E\]** &ndash; The user interface will ask if the engine has any reservations to announce<br/>
    *TODO: long description*

* **`solo?` \[U&rarr;E\]** &ndash; The user interface checks if the engine wants to announce a solo<br/>
    *TODO: long description*

* **`pigglets?` \[U&rarr;E\]** &ndash; The user interface checks if the engine wants to announce pigglets<br/>
    *TODO: long description*

* **`wedding?` \[U&rarr;E\]** &ndash; The user interface checks if the engine wants to announce a wedding<br/>
    *TODO: long description*

#### Announcements
* **`re/contra` \[E&rarr;U\]** &ndash; The engine announces re/contra, no 90, no 60, no 30, or black<br/>
    *TODO: long description*
    
* **`re/contra <pid>` \[U&rarr;E\]** &ndash; An engine has announced re/contra, no 90, no 60, no 30, or black<br/>
    *TODO: long description*

#### Place card
* **`place <card>` \[E&rarr;U\]** &ndash; The engine places the mentioned card off its hand<br/>
    *TODO: long description*
    
* **`place <pid> <card>` \[U&rarr;E\]** &ndash; An engine has made its move<br/>
    *TODO: long description*

## Examples
> TODO