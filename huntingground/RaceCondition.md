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
d) Burp Suite - Extracting all state changing endpoints 

The Attack Workflow:
1. Identify a multistep operation
2. Find the state changing endpoint
3. Check if the operation is atomic - send two request manually at the same time
4. Exploit parallel requests
5. Observe backend behaviour
6. Stabilize the exploit

Extracting State Changing Endpoints
A state changing endpoint is any endpoint where the server modifies data, updates a resource or performs a multi-step operation.
1. Map all POST/PUT/PATCH/DELETE endpoints - These are the primary suspects. You should also inspect GET endpoints that trigger server side actions, background job triggers and webhook receivers
2. Identify endpoints that modify money, points or inventory - they are the highest value race targets
3. Extract Endpoints involved in multi-step workflow
4. Find endpoints that update user state - these often lack locking and can be raced to bypass verification
5. Look for endpoints thatcreate or delete resources
6. Identify endpoints that modify server side counters - counters are almost always vulnerable
7. Extract endpoints that perform idempotency checks - If the backend uses : X-idepondency key, ETag, If-Match, version numbers . These are optimistic locking mechanisms - and they are often bypassable

Prioritization of the Endpoints
| Endpoint Type | Priority | Why |
| --- | --- | --- |
| **Money-related** | 🔥🔥🔥 | Direct financial impact |
| **Inventory-related** | 🔥🔥 | Double-purchase, oversell |
| **Coupon/discount** | 🔥🔥 | Double redemption |
| **Account/identity** | 🔥 | Account takeover vectors |
| **Profile updates** | Medium | TOCTOU opportunities |
| **Counters** | Medium | Rate-limit bypass |
| **Misc state changes** | Low | Rarely exploitable |

Procedure to Extracting State Changing Endpoints From Burp Traffic
1. Filter for mutating HTTP Methods
	In Burp -> Proxy -> HTTP history:
	Use the filter bar and select
	a) POST
	b) PUT
	c) PATCH
	d) DELETE
	These are the primary state changing endpoints and instantly removes 70% irrelevant noise
	On the filter window
	i) Enable show only parameterized requests - this removes static noise and keeps only requests with JSON bodies, Form Data and Query parameters
	ii) Filter by HTTP Method
	iii) Filter by MIME type to remove static assets - HTML , Script, XML and Other Texts
	iv) Use file extensions to remove static files - js, gif, jpg, png, css,woff, woff2
	v) Enable Hide items without responses
	Once filtered :
	a) Sort by request body size ( request length )- larger bodies = more logic = more race windows. 	
		0-300 - usually noise
		300 - 1500 - interesting
		1500 - 10,000 - high value
		10,000+ - multistep logic , file uploads, payment flows
	b) Sort by response status - 200 + 400 mix = inconsistent behaviour - raceable. This is a mixture oof success and failure codes. And if the same request sometimes succeeds and sometimes fails :
		1. The backend is doing multistep operations
		2. The operations are not atomic
		3. The system timing- dependent
		4. You have a race window
	c) Sort by time - endpoints that take longer to respond often have multistep logic - raceable. You llook at the time column. If it is not there right click the column header and enable time in ms
		1. 300ms - likely multistep logic
		2. 500ms - very likely multistep logic
		3. 1000ms - jackpot
	Do not use the start responce timer . That is for the repeater not traffic analysis

Replaying the Endpoints Manually:
You should make sure the endpoints are from the platform
It means : Take the exact request from HTTP history, send it to repeater - send it multiple times - observe how the backend behaves. You are checking whether the backend behaves consistently or inconsistently when you send the same request repeatedly . The fast ways to detect:
	a) Non-atomic operations
	b) Multi step logic
	c) Timing dependent behaviour
	d) Race windows
When comparing the responses we look for:
	a) Status codes
	b) Response bodies
	c) Timestamps
	d) IDs
	e) Counters
	f) Messages
	g) Any variation
If there are any variations the endpoint is raceable

You can also check the Dev tools network console to determine if the processes are atomic or not
The presence of several calls on the network tab shows that the process is not atomic but that does not guarantee that there is a race conditon because the backend may not be like the front end .
Even after finding that thebackend is multistep that simply means that a race window exist the next step is to determine if the backend protects those steps or does not. It can be protected in the following ways
	a) Backend uniqueness constraints
	b) Backend locking
	c) Backend transactions
	d) Backend idempotency
For this you need to learn how to interpret the dev tools properly

How to determine if a backend is atomic
a) It exposes only one endpoint
