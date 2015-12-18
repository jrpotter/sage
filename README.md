Sage
====

[Version 1.0.0 - 12/15/2015]

Sage is a packrat PEG parser I created for the intents of building my own language. It provides support for parsing
generic PEG grammars (some examples included in the /grammars folder).

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
  * Can then begin parsing an arbitrary file according to this grammar, returning an AST

Limitations
-----------

This is primarily an experimental project, and I believe I've run the course of the experiment, especially after
studying up on unicode functionality. Provided are some of the limitations of Sage if you decide to play with it:

* Unicode Support
  * While I could perhaps look into getting this running, it would be an insane amount of work to try and compete against
    something like ICU or Boost.Locale. And if I were to settle with either of these libraries, I might as well use their
    corresponding regex libraries as well (which also supports unicode).
* Contextual Analysis
  * I would like to incorporate some means of declaring types within the PEG file so that Sage can perform the contextual
    analysis but its not something I'm too excited about jumping on quite yet.
* Scanning
  * If a language is whitespace delimited (something like Python), the provided Scanner provides very limited support for
    this.

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
