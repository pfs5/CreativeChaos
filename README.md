# Creative Chaos
A fast and streamlined task manager to tame the chaos of your daily work.

## Why?
I'm a software engineer. Software engineers have a lot of work to do but, even though we can easily manage complex systems and multi-level dependency chains, we might not be the most organized people in the world.

I have no problem being productive as long as I know what I am supposed to be productive about. I've tried many task management solutions - from employer enforced JIRA to self-imposed Trello or Obsidian, yet I always fall back to good old post-it notes and my trusty notebook.

So I did the classic programmer move and decided to implement it myself. It might take slightly longer, as side projects usually do,  yet I can tune it to my needs precisely. If someone happens to have similar wishes to me, my tool might even have more than one user.

## What?
I started with some **hard constraints**:
* **Simple workflow** - Add task, rename task, close task. Doable in one or two steps. Advanced features are hidden away.
* **Keyboard focused** - I like using my keyboard and I'm usually faster than with the mouse. Shortcuts are the way to go!
* **Fast and responsive** - I want immediate feedback. No loading, no waiting. Even crashes must be quick!

I also formulated some **crucial features** I want to have:
* **Task list** - Kinda obvious, this one. I want it simple, readable and useful.
* **Add/Remove/Rename task** - Basic task operations must be quick and easy.
* **Task grouping** - I knew I needed to organize my tasks somehow so I can prioritize. I wanted to explore different ideas. 
* **Time tracking** - I can activate/deactivate tasks and see how long the task has been active for.

And that's it! That was enough to get me started.

## How?
First, I did the sensible thing and started from scratch, and I mean *from scratch*. I started from blank window and begun implementing a GUI framework. I even designed the interface, with interactions and all. 

Halfway through I realized implementing a GUI framework is hard and I will never finish. Back to the drawing board.

If you like to know how far I've gotten, here's the [repo](https://github.com/pfs5/TaskOrganizer). I wrote around 2500 lines of code and got this far. If you're into music production, you might recognize where I "borrowed" the design from.

Plan:
![Idea:](https://github.com/pfs5/CreativeChaos/assets/14861165/6dd30943-4873-414f-b153-cd93a4404d74)


Reality:
![Reality:](https://github.com/pfs5/CreativeChaos/assets/14861165/259a28fe-9cfb-4fd5-a7e6-59d8d46e5a81)


I finally decided to use [ImGUI](https://github.com/ocornut/imgui/releases) - a beautiful, powerful and simple to use GUI framework which I've had the pleasure of using before. I also needed to save all of the data for which I used this excellent [JSON library](https://github.com/nlohmann/json). Everything else I'll do myself (for now).

## Result
At the point of writing this (March 2024) I have implemented the basic feature list and actually started to use my tool daily. I constrained myself to iterate - implement a usable tool first and work on improvements later. One might easily get lost in implementing a custom std::vector class for that nice 1 nanosecond speed gain I didn't let myself get seduced. I did, however, leave "todo"-s everywhere for my future self to handle later.

Here's an overview of the current features.
![](https://github.com/pfs5/CreativeChaos/blob/master/README/ft-overview.gif)
