#**Tasks Program Design:**
-------
###Data structures:
>- **Task**:
	- date/time
		- date
		- time
		- due date enabled
	- name
	- Description
	- category
-------
###Technicalities:
User create a list of tasks and apply a catagory to them.  They should be able to filter tasks by category.

&middot;  In the tasklist, tasks that are due today have an asterisk before them:

 *_\*task*

&middot;  Any task overdue will have a ">*" before it:

*\>\*task*

The user will scroll up/down the list using a windowed scroll pane of the list.  They should be able to delete, add, and modify any tas that they select.

---

---

---

##*Program Design Guidlines*:

- NO GOTOs!
- Functions are written **as needed**, this means loaders/savers will be written last for the sake of functionality and minimalism.
- No **singletons**
- no **static variables**
- all functions not used in another file should be namespaced to limit the scope to the file said function is used in
- functions used in only single files shall be inlined
