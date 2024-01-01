# Crucible 

---

## What is this project?
A couch Co-Op 2D top-down stealth rogue-like game.

---

## Tools
- C++
- SFML
- CMake

---

## Dev Log

### Entry #1 [Nov 14th -> Nov 26th]:
- Basic 2D geometry added to the world.
- Basic player controls.
- Ability to push and move objects in the world.
- Introduction of entity memory pooling.

I did not take screenshots during this period.

---

### Entry #2 [Nov 27th -> Dec 30th]:
Not a huge amount of time was spent working on the project during this period due to work and with the Christmas holidays.
However, I did start a separate branch on the project for building my own line segment intersection algorithm.

The goal with this was to add a visibility system to the game so we could determine what is visible to a given entity.

This went through many changes as a result of the wrong strategy being used. The initial implementation worked for
a set of rays in a radial pattern around the player entity. However, when joining these rays with a TriangleFan there 
was a lot of jagged edges, flickering and clipping over the corners of other world entities. Eventually, I managed to
succeed in getting this to work. 

Here are the changes summarised during this period:
- Visibility System with Line Segment Intersection

**Screenshots**
![Sat 30 Dec 2023 16:50:20 GMT](https://github.com/Ticketedmoon/Crucible/assets/21260839/1ef528d2-6c34-4128-b85f-1549b1c84021)
![Sat 30 Dec 2023 16:57:20 GMT](https://github.com/Ticketedmoon/Crucible/assets/21260839/4e6bc14b-58e1-4ffb-88fd-89ae573f7f57)

- The pinky block represents our player and light source. 
- The yellow area represents the region the player can see.

---

### Entry #3:
[TBD]

---

### Credit
- https://ncase.me/sight-and-light/
