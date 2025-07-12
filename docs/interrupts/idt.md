# Interrupts, IDT, ISRs, and IRQs – Overview

Modern operating systems rely on **interrupts** to efficiently respond to hardware and software events. This document provides a high-level overview of the key components involved in handling interrupts in x86 protected mode: the **Interrupt Descriptor Table (IDT)**, **Interrupt Service Routines (ISRs)**, and **Interrupt Requests (IRQs)**.

---

## What Are Interrupts?

An **interrupt** is a signal sent to the CPU that temporarily halts the current execution flow so the CPU can **handle an event**. Examples include:

- A key being pressed on the keyboard
- A timer reaching a specific count
- A program causing an error (like division by zero)

Interrupts allow the CPU to respond immediately instead of constantly checking if something happened (**polling**).

---

## The Interrupt Descriptor Table (IDT)

The **IDT** is a data structure used by the CPU in **protected mode** to find the correct code to run when an interrupt occurs.

- Contains up to **256 entries**
- Each entry corresponds to an **interrupt number**
- Tells the CPU which function (ISR) to call for a given interrupt

> 📌 For example: When the keyboard triggers interrupt `0x21`, the CPU uses entry `0x21` in the IDT to find and call the handler.

---

## ISRs – Interrupt Service Routines

An **Interrupt Service Routine (ISR)** is the function that runs when a specific interrupt occurs. Its job is to:

- Save the CPU state (registers)
- Perform the required logic (e.g., read a key, increment a tick count)
- Send an **EOI (End of Interrupt)** to the PIC
- Restore CPU state and return with `iret`

Each interrupt we care about (e.g., keyboard or timer) gets a corresponding ISR (e.g., `isr_1` for IRQ1).

---

## IRQs – Interrupt Requests

**IRQs** are hardware-level interrupt lines used by devices to request CPU attention.

- Managed by the **Programmable Interrupt Controller (PIC)**
- By default, IRQ0–IRQ15 are mapped to interrupt vectors **0x08–0x0F**
- We **remap** them to **0x20–0x2F** to avoid overlapping CPU exceptions

| IRQ | Device       | Mapped to |
|-----|--------------|-----------|
| 0   | System timer | 0x20      |
| 1   | Keyboard     | 0x21      |
| 2–15| Other devices| 0x22–0x2F |

---

## Why Is This Useful?

Interrupts allow the OS to:

- Respond in real time to hardware events
- Schedule and switch tasks using timers
- Handle exceptions (e.g., divide-by-zero)
- Avoid inefficient polling

---

## Example: Pressing a Key

1. You press a key.
2. The **keyboard controller** sends **IRQ1** to the PIC.
3. The PIC maps IRQ1 to **interrupt 0x21**.
4. The CPU looks up the handler in the **IDT**.
5. The ISR (`isr_1`) is executed:
   - Reads the scancode from port `0x60`
   - Converts it to ASCII
   - Prints the character
   - Sends **EOI** to PIC
   - Returns control back

---

## Quick Summary

| Term  | Stands For                  | Purpose                                     |
|-------|-----------------------------|---------------------------------------------|
| IDT   | Interrupt Descriptor Table  | Maps interrupt numbers to handler addresses |
| ISR   | Interrupt Service Routine   | Code that runs when an interrupt occurs     |
| IRQ   | Interrupt Request           | Signal from a hardware device               |
| PIC   | Programmable Interrupt Ctrl | Routes IRQs to interrupt vectors            |

> 📘 Each topic here will have its own detailed documentation page later.

