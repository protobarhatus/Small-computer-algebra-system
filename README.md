# Computer algebra system
## About
This is my project made in high school on C++ with using QT framework. This is the system of computer algebra, like Wolfram Mathematica. Although it is by far less powerful since its just a school project, it can work with complex expressions with all elementary functions, vectors, matrixes, perform symbolic differentiation and integration, solve (analiticly) differential equations and do some tasks with geometry. Here is a brief overview of the program.
## General overview and how to work with the program
The main window is pretty much minimalistic:
![image](https://user-images.githubusercontent.com/51193423/228551006-a3ede81d-84a3-4d0d-aac5-dc7657f65690.png)

The upper text box is used for input. Here user can write requests. There are several types of them, but more on that later. The output will be displayed in the big box down below, for each independent command there will be separate result. If command has a mistake, the error message will appear near the command, and in output it will be ignored:
![image](https://user-images.githubusercontent.com/51193423/228555295-e36366c1-226e-4b78-98c3-e50a2e0494bd.png)

By the tabs above one can switch between differenct scripts, which are completly independent from each other.
Also, user can switch between output modes: to render them like latex, or get a wolfram-compatible string.

## An overview of language
In this section i will write about rules of the language, but not about every function that exists.
One note before detalisation is that brackets () and [] are fully interchangeable, but opening and closing bracket in pair should be of one type.
### Types of expressions
There are several types of expressions that can be made and program will respond differently to them:
1. Just mathematical expression. This is base type of request, and when program gets one, it will try to simplify it and output simplified version. Any requests that does not fall for other categories will be considered as this type. It can consists of any correct compinations of functions, variables, numbers, vectors, matrixes, etc
2. Variable assignation. This request must have this sintax:
```
<Name of variable> = <Mathematical expression>
```
After that, all code below will consider that this variable has stated value. If variable didn't exist above, this expression will create it, otherwise value will be rewritten. Note that this doesn't influence code above expression. Mathematical expression at the right may be any correct expression, can include other variables.
3. Function assignation. This is similar to previous one, but the sintax must be like:
```
<Name of function>[argument1, argument2, ...] = <Mathematical expression with arguments>
```
Amount of arguments can be any, but not zero. Argument is just the name, no types declarations. Name can be anything, the only condition is that there should not be any variable above with this name.
4. Request to solve equation (not differential). It can be made by one of these commands:
```
<Mathematical expression> == <Mathematical expression>
```
But this will work only if it contains only one variable. Otherwise, this should be used:
```
SolveFor[<Mathematical expression> == <Mathematical expression>, <name of variable for whom it should be solved>]
```
Sadly, now there is no support for systems, although code for them exists in the back, im too lazy to write an interface for it.
5. Request to solve differential equation. It can be made by this command:
```
<Mathematical expression> == <Mathematical expression>
```
It will be considered as differential if there are `dx` and `dy`, or `y'` or any `y<any amount of strokes>` somewhere in expressions. Here the can be any amount of variables that will be considered as parameters (though they very complicate solving process).

### Types of entities
1. Numbers. The simplest type. They can be integers, rational in form of fraction or some part of other expressions, like `sqrt(2)`.
2. Symbolic variables. It is some name that can participate in expression. They are also considered as simpplest form of themselves and they behave just like some unknown variables in math (like a + 3 does not simplify, a+a=2a and etc). By default, all english lower-case letters, except "e" are symbolic variables and cannot be changed (letter "e" is euler constant), but any simbolic variable can be accessed by function var("Name") (For example, `var("ABC") + 3` is `ABC + 3`). Sadly, they can be considered only as one-dimensional "numbers" (so, `a + {1; 2}` will give an error, but `{a; b} + {1; 2}` will not).
3. Variables that hold value. The value can be any entity. They can be created by assignation expressions and in all expressions their value is substituted into expression. (They can be made as symbolic by `Name = var("Name")` command.
4. Elementary functions. They are `sin`, `cos`, `tan`, `cot`, `ln`, `asin`, `acot`, `atan`, `acot`. They participate in expression symbolicly, their argument does not substitude anywhere. (Although, for example, `sin(pi/2)` will be simplified to `1`).
5. User-defined functions. They can be made by function assignment request and, when called, their arguments are substituted in expression.
6. Built-in functions. (For example, inverse, factorize, etc). They usually have specific tasks, they take definced arguments and return their result. Ofcourse, they can be called in the middle of complex expression.
7. Vectors. They are set of one-dimensional entities end can be created as `{a; b; c; ...}`.
8. Matrix. Two-dimensional set of one dimensional entities. Can be created as `{V1; V2; ...}`, where `V1, V2, ...` - vectors of the same length.
9. Geometrical entities - there are several types of them, but more on it later.

## Some built-in functions examples
![image](https://user-images.githubusercontent.com/51193423/228587991-4dd3e090-733d-4cd8-9169-21058df3f5b2.png)

## Solving differential equation example
For a differential equation, the program will not only solve it, but also provide detailed description of solving process:
![image](https://user-images.githubusercontent.com/51193423/228588894-7d22ffb5-7a71-4686-92f8-b9b8c178eb32.png)

![image](https://user-images.githubusercontent.com/51193423/228589026-37382928-274f-4855-8456-fd29b55b4f1d.png)
![image](https://user-images.githubusercontent.com/51193423/228589566-ee199076-c234-4a3d-bdbb-7f8108b21ce3.png)

