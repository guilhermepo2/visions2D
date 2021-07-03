#pragma once
#include <memory>

struct AllocationMetrics {
	unsigned int TotalAllocated = 0;
	unsigned int TotalFreed = 0;

	unsigned int GetCurrentUsage() const { return TotalAllocated - TotalFreed; }
};

static AllocationMetrics s_CurrentMetric;

void* operator new(size_t size) {
	s_CurrentMetric.TotalAllocated += size;

	return malloc(size);
}

void operator delete(void* memory, size_t size) {
	s_CurrentMetric.TotalFreed += size;

	free(memory);
}