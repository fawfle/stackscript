# CSCI 2275 - Final Project: Problem‑Solving with a Chosen Data Structure

## Overview
Design and implement a solution to a **real, non‑trivial problem** using a **data structure of your choice** (or a small combination where one is primary). You will justify why your chosen DS is appropriate, build a correct and efficient implementation, and evaluate it **asymptotically** (and **emperically** if possible).

You may work **solo or in pairs**. If you choose to work in pairs, please notify me through Piazza of your pair.

---

## Learning Objectives
By the end of this project, you will be able to:
1. **Frame a problem** and identify core operations & constraints.
2. **Select and justify** a data structure based on operation costs.
3. **Implement** the data structure and its algorithms with attention to correctness and edge cases.
4. **Analyze complexity** (Big‑O) for operations and overall workflow.
5. **Benchmark empirically** and interpret results against asymptotic predictions.
7. **Communicate** design trade‑offs in your demo presentation.

---

## Allowed Tech
- Languages: C++
- You must provide simple build/run instructions (Makefile).
- External libraries: You may use any STL for the purposes of your project except the ones for the data structures (eg: stacks, queues, hashmaps etc.)

---

## Project Tracks (Examples)
Choose **one** problem, or propose your own.

1. Social Network App that allows users to create an account and find, add and remove friends. You need to add at least 1-2 features of your choosing.

2. URL Shortener App that helps map long URLs to short hashes and handle collisions. You need to ensure that no 2 URLs are mapped to the same hash. You need to build your own hash function.

3. Pathfinder App that allows users to find the best path from one location to another. Consider that there is a cost (time, duration, effort) for every sub path that builds the final path. You app should allow users to add locations in the map and provide the connections between the new location and the existing locations in the map.

4. Web Browser Navigation app that simulates a browser's back/forward buttons to track navigation history

5. Mathematical expression evaluator app that parses and computes mathematical expressions and provides the result.

6. Compute the average wait time of a Theme Park Ride. The app should simulate multiple rides where visitors enter queues based on arrival time and ride capacity.

7. Spell Checker App that store words for quick lookup. Implement functions to find whether a word exists, suggest nearest matches (based on alphabetic proximity), and handle prefix searches.

> You may also propose domain problems from finance, games, or your own projects—just make sure the **data structure is the star**.

---

## Milestones & Deliverables

### Milestone 1 — Proposal (Due Nov 14, 2025 11:59 p.m.) - You will receive feedback by Nov 17th, 2025
- **Problem statement**: What user need does this solve?
- **Operations**: Exact operations you intend to include in your solution.
- **Chosen DS & justification**: Why it fits better than alternatives.

**Submit** `proposal.pdf` to your repository.

### Milestone 2 — Presentation (Due during lectures in the week of Dec 1-5, 2025)
- Minimal vertical slice working: core DS operations + 2–3 passing unit tests.
- Include `design.md` skeleton with invariants and operation cost table.

### Milestone 3 — Final Submission (Due Dec 5, 2025 11:59 p.m.)
**Repo structure (required):**
```
final-project/
  src/                # your source code
  docs/
    proposal.md
    report.md         # 4–6 pages
    genai_usage.md    # prompts/outputs if GenAI used
  Makefile            # you can take help from the professor or TA to write this
  README.md           # build/run/test/benchmark instructions
```

**Final deliverables:**
- **Working code** that builds from a clean clone.
- **Complexity analysis**: Per operation.
- **Report (4–6 pages)**: Problem, DS design, alternatives considered, analysis, benchmarks, limitations, future work.
- **Short demo (5 min)**: Live; show correctness and performance.

---

## Assessment (Points‑Based)
Total: **100 points**

1. **Problem framing & DS justification (20)**  
   Clear operations, constraints, and why the chosen DS dominates alternatives.

2. **Design (15)**  
   Sound design choices for the solution including correct usage of OOPs concepts.

3. **Implementation correctness (20)**  
   Passes functional tests; handles edge cases; avoids runtime errors.

4. **Analysis (15)**  
   Sound asymptotic analysis; ties to access patterns; trade‑offs discussed.

5. **Code quality & docs (15)**  
   Readability, structure, comments, README clarity.

6. **Presentation/demo (15)**  
   Concise, compelling, shows correctness + performance.

**Academic integrity violations: up to −100 and referral to honor code.**

---

## Proposal Form (Template)
Include in `proposal.md`:
1. **Title** & 1‑sentence elevator pitch.  
2. **Problem & users** (≤ 150 words).  
3. **Operations** table with ops.
4. **Chosen DS** and **why** (compare two alternatives).  

---

## Report (4–6 pages) Outline
1. Problem & requirements  
2. Related DS alternatives (pros/cons)
3. Operation algorithms & complexity  
4. Testing strategy  
5. Discussion: when your DS shines/fails  
6. Limitations & future work

---

## GenAI Usage Policy
GenAI tools may be used for **scaffolding, brainstorming, refactoring, documentation, and plotting**, but **not** to implement the core data structure or algorithms verbatim. If GenAI is used:
- Include a `docs/genai_usage.md` with **all prompts** and **key outputs** used.  
- Add a **reflection** on how you validated or corrected GenAI suggestions.  
- Cite any generated text/snippets in code comments.  
- You are responsible for correctness and academic integrity.

---

## Submission
- Submit your work to the GitHub repository by Dec 5, 11:59 p.m.

---

## Presentation Day
- **5 minutes** per team.  
- Show: (1) the problem, (2) the solution with the DS of your choosing and an operation demo, (3) one interesting bug you found and fixed.  
- Be ready for 2 minutes of Q&A on trade‑offs.

---

_Disclaimer: Generative AI was used to proof the text of this assignment._
