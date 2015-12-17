Sage
====

[Version 0.0.1 - 12/15/2015]

Sage is a packrat PEG parser I created for the intents of building my own language, Guru. It is still in the works but is
nearing completition and provides the framework needed to parse arbitrary PEG grammars.

Features
--------

* Full fledged Regex support
  * I am aware C++11 provides support for regexes but wanted to custom-roll my own library
  * This allows me to have complete control over the scanning and parsing process
  * This was also for my own education of NFA -> DFA -> Regex conversions
* Arbitrary Scanning
  * Scanning is provided in a manner similar to the Java Scanner
  * Allows for reading in delimited tokens (and not if not word bounded)
* PEG Parsing
  * By using the PEGParser class, one can construct a PEG parser from a .peg file
  * Can then begin parsing an arbitrary file according to this grammar, returning an AST (in progress)

Example
-------

The following grammar is also written out in arithmetic.peg under the /grammars folder:

```
Expression' -> Sum
Sum         -> Product ("[+\-]" Product)*
Product     -> Value ("[*/]" Value)*
Value       -> "[0-9\"]+" | "\(" Expression "\)"
```

Here a PEG statement is broken up into a term (the nonterminal) and the definition (everything to the right of the
arrow operator). To refer to another nonterminal, simply call this nonterminal by name. To refer to a terminal, surround
a string via quotes and place in a regex (according to the rules of the custom regex module). Note that regexes and
nonterminals can also be surrounded by parenthesis and repeated with the kleene star ('*') and plus operator ('+'), or 
made optional ('?').

The above would parse the following statement:

```
195 + (186 * 32) - 14 / 9
```

and construct the following AST tree:

```
|- Sum
|---------- Product
|-------------------- Value
|------------------------- 195
|-------------------- +
|-------------------- Product
|------------------------------ Value
|---------------------------------------- (
|---------------------------------------- Expression
|--------------------------------------------- Sum
|------------------------------------------------------- Product
|----------------------------------------------------------------- Value
|---------------------------------------------------------------------- 186
|---------------------------------------------------------------------- *
|---------------------------------------------------------------------- Value
|--------------------------------------------------------------------------- 32
|---------------------------------------- )
|-------------------- -
|-------------------- Product
|------------------------------ Value
|----------------------------------- 14
|----------------------------------- /
|----------------------------------- Value
|---------------------------------------- 9
```
