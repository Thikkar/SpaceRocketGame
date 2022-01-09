# SpaceRocketGame
A game based on Galaga, completed as a final project for CS211 at Northwestern University

Objective: Survive as long as possible. Shoot incoming asteroids, monsters, and enemy spaceships to gain points. Acquire health boosts to regenerate lost health.

Rules: 
  1) W,A,S,D to move. Space to shoot.
  2) Colliding with an enemy spaceship results in an instant death.
  3) Colliding with asteroid or monster, or getting shot by an enemy spaceship deals 10 HP of damage.
  4) Collecting a health boost regenerates 10 HP or full health, whichever is smaller.
  5) Monsters can attack with varying velocities.
  6) If player's spaceship moves past the top of the window, it will reappear from the bottom at the same x-coordinate (and vice versa). 
     If player's spaceship moves past the right of the window, it will reappear from the left at the same y-coordinate (and vice versa).
     Monsters and enemy spaceships that go past the edge of our window disappear and do not reappear on the opposite side of the screen.
     Asteroids will reflect vertically/horizontally when hitting the edge.
  7) White circles = Asteroids; Squares = monsters (different color = different velocity)
