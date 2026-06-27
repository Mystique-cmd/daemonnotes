The targets are:
a) State Changing Endpoints
b) Non-atomic Operations

The attack criteria is requests that are:
a) Parralel
b) High speed
c) Synchronized

A race conditions happens when:
a) A system performs a multi-step operation
b) Those steps are not atomic
c) Two or more requests hit the system at the same time
d) The system processes them in an unexpected order
e) You get state corruption, double execution or bypass

The types of Race conditions to hunt:
a) State manipulation races - occurs when two requests modify the same resource at the same time
b) TOCTOU Races - Time of Check Time of Use. Occurs when the system checks something, then it uses it later - and you can change it in between
c) Locking / Concurrency Races - Occurs when the backend fails to lock a resource or uses weak locking

Where race conditions hide:
a) Balance Updates
b) Coupon redemption
c) Reward Points
d) Inventory Deduction
e) Payment Initiation
f) Email / Phone Verification
g) Password reset flows
h) Account creation
i) File uploading processing

The toolkit:
a) Turbo Intruder - for high speed parallel requests
b) ffuf threading - for brute forcing concurrency windows
c) Custom python async scripts - For precise timing and payload control

The Attack Workflow:
1. Identify a multistep operation
2. Find the state changing endpoint
3. Check if the operation is atomic - send two request manually at the same time
4. Exploit parallel requests
5. Observe backend behaviour
6. Stabilize the exploit
