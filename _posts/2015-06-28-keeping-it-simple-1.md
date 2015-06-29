---
layout: post
title: "Keeping It Simple and Clean (Part 1)"
description: ""
category:
tags: [Simple, Software Engineering, Engineering Philosophy, Design Patterns]
---
{% include JB/setup %}

When I started programming circa 2000 as a hobby, all I cared about was how cool my programs and games were. It was mostly about using whatever tools I had at my disposal to make them happen. Participating in programming contests later turned me into a sloppy programmer that would take clever shortcuts, and see every small problem as an opportunity to apply a complicated cocktail of data structures and algorithms that I'd learned at school. That was sometimes the only way to solve some of those problems, but nothing at school or those contests prepared me for the great lesson that I would learn in real-world programming.


*"I didn't have time to write a short letter, so I wrote a long one instead.”* - Mark Twain


Writing simple letters is not easy. Writing simple programs is not easy either. I learned that taking pride in the ability to make complicated contraptions that accomplish a simple task is a sign of professional immaturity. I learned an engineer who can keep things as simple as possible is the real programming artist. No expression could articulate it any better than Antoine de Saint Exupéry's if I'm allowed another quote from another great author: *"It seems that perfection is reached not when there is nothing left to add, but when there is nothing left to take away”*.

This principle can almost perfectly translate to engineering. The [KISS principle](http://en.wikipedia.org/wiki/KISS_principle) ("Keep It Simple, Stupid!") is a great example of putting simplicity first. As a more software-engineering-centric example, Agile principle #10 states that *"simplicity, the art of maximizing the amount of work not done, is essential"*. One would ask what simplicity means in software, and how it could be achieved. Let's see if we can find the answer.

### Simplicity in software

There's been extensive academic research, and many metrics were invented by academics and good-old general line managers to measure software productivity, complexity, and value. The general consensus is that software is too complex to be accurately quantified, but there are oversimplified metrics that can still tame some of that complexity. Lines of code is probably one of the first metrics that was used in the industry. It's very easy to measure and it correlates well with complexity, but it's not too difficult to make some condensed piece of software that no one can understand and cannot be legitimately considered simple.

Other metrics usually try to quantify software complexity in terms of branches, components, number of interfaces, or moving parts in general. As this is not an academic post, and I don't have enough authority to analyze them, let's just leave the nitty-gritty details to academics and admit that simplicity to the average developer Joe is more of an art than science. Maybe the hipster artisanal JavaScript programmer at your local Starbucks can get it right easier than the Ivy school PhD student after all.

### How to achieve simplicity

You must have realized by now that there's no silver bullet to achieve simplicity. There are however tools and patterns that make it easier for us to eradicate complexity in software, one wrong design decision at a time. I'll enumerate some of the tricks, but it won't be a comprehensive list by any means.

#### Understand the damned code first

Chances are you're not creating everything from scratch. You're working on something that a few other poor souls put together, and if you don't want to be sticking more incongruous mud on the pile, you'll need to understand a thing or two about the code base. This will first and foremost keep you from re-inventing the wheel; someone before you probably encountered a similar problem and spent time to fix it properly, so why not make use of that?

Another important measure, especially when fixing bugs, is to understand the root cause of the unwanted behaviour. If you don't kill the bug where it lives, and just try to stick a band-aid on the symptoms, you're picking an uphill battle with a bug that you don't even fully know. In a well-designed code base, the root cause should ideally be in one place, so you'd need to address it in a single place instead of chasing it around and playing whack-a-mole with every symptom of the bug.

It always pays off to at least have a basic understanding of what every major component in code does. This helps you make proper use of them. Misusing or abusing other components will make the software more fragile, since other developers cannot always guess unorthodox application of their components and every change can potentially pull the rug from under your code.

#### Encapsulation

The integrity of systems is better protected if their boundaries are well defined. Logical components of software should also have their own boundaries. Encapsulation is facilitated and enforced in some object-oriented languages, but in general, some discipline will make it possible in every language. Classes, private fields and subroutines are examples of encapsulation tools that can be implemented in languages as wildly loose as JavaScript.

The best way to achieve encapsulation is to define modules, clearly define the boundaries, and separate the concerns of those modules. This is a very high-level recipe, and in practice, things can get sticky and impossible to separate. However, it should be obvious to software engineers, when deciding where to put anything, whether defining the boundaries will add any value or it's turning into another anally-retentive obsession with how to store your possessions in one of the 100 drawers in the bedroom.


*(to be continued...)*