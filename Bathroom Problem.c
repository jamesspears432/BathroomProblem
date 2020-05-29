#include <Windows.h>
#include <stdio.h>

/* semaphore initialization
room = 1; // room == 0, bathroom empty
male = 5;
female = 5;
turnstile = 1; // prevent starvation
mutexm = 1; // control male
mutexf = 1; // control female
mutex = 1; // control result output
*/

int m = 0, fm = 0; // number in bathroom
HANDLE room, male, female, turnstile, mutexm, mutexf, mutex;
FILE* f;

void males() {
	int t; // time using the bathroom
	while (1) {
		WaitForSingleObject(turnstile, INFINITE);
		WaitForSingleObject(mutexm, INFINITE);
		m++;
		if (m == 1) WaitForSingleObject(room, INFINITE);
		ReleaseSemaphore(mutexm, 1, NULL);
		ReleaseSemaphore(turnstile, 1, NULL);

		// Enter the bathroom
		WaitForSingleObject(male, INFINITE);
		WaitForSingleObject(mutex, INFINITE);
		printf_s("Male in\n");
		/*f = fopen_s(&f, "result.txt", "a+");
		fprintf(f, "Male in\n");
		fclose(f);*/
		ReleaseSemaphore(mutex, 1, NULL);
		t = rand() % 5000;
		Sleep(t);

		// Leave the bathroom
		WaitForSingleObject(mutex, INFINITE);
		printf_s("Male out\n");
		/*f = fopen_s(&f, "result.txt", "a+");
		fprintf(f, "Male out\n");
		fclose(f);*/
		ReleaseSemaphore(mutex, 1, NULL);
		ReleaseSemaphore(male, 1, NULL);
		WaitForSingleObject(mutexm, INFINITE);
		m--;
		if (m == 0) ReleaseSemaphore(room, 1, NULL);
		ReleaseSemaphore(mutexm, 1, NULL);
	}
}

void females() {
	int t;
	while (1) {
		WaitForSingleObject(turnstile, INFINITE);
		WaitForSingleObject(mutexf, INFINITE);
		fm++;
		if (fm == 1) WaitForSingleObject(room, INFINITE);
		ReleaseSemaphore(mutexf, 1, NULL);
		ReleaseSemaphore(turnstile, 1, NULL);

		// Enter the bathroom
		WaitForSingleObject(female, INFINITE);
		WaitForSingleObject(mutex, INFINITE);
		printf_s("Female in\n");
		/*f = fopen_s(&f, "result.txt", "a+");
		fprintf(f, "Female in\n");
		fclose(f);*/
		ReleaseSemaphore(mutex, 1, NULL);
		t = rand() % 5000;
		Sleep(t);

		// Leave the bathroom
		WaitForSingleObject(mutex, INFINITE);
		printf_s("Female out\n");
		/*f = fopen_s(&f, "result.txt", "a+");
		fprintf(f, "Female out\n");
		fclose(f);*/
		ReleaseSemaphore(mutex, 1, NULL);
		ReleaseSemaphore(female, 1, NULL);
		WaitForSingleObject(mutexf, INFINITE);
		fm--;
		if (fm == 0) ReleaseSemaphore(room, 1, NULL);
		ReleaseSemaphore(mutexf, 1, NULL);
	}
}

int main() {
	HANDLE h1[10], h2[10];
	DWORD Id;
	int i;
	srand(time(0));
	mutex = CreateSemaphore(NULL, 1, 1, NULL);
	room = CreateSemaphore(NULL, 1, 1, NULL);
	turnstile = CreateSemaphore(NULL, 1, 1, NULL);
	mutexm = CreateSemaphore(NULL, 1, 1, NULL);
	mutexf = CreateSemaphore(NULL, 1, 1, NULL);
	male = CreateSemaphore(NULL, 5, 5, NULL);
	female = CreateSemaphore(NULL, 5, 5, NULL);

	for (i = 0; i < 10; i++)
		h1[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)females, NULL, 0, &Id);

	for (i = 0; i < 10; i++)
		h1[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)males, NULL, 0, &Id);
	
	WaitForMultipleObjects(10, h1, TRUE, INFINITE);
	WaitForMultipleObjects(10, h2, TRUE, INFINITE);
	getch();
	return 0;
}