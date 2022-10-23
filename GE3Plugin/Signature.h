#pragma once

// Signature scan in specified memory region
extern void* sigScan(const char* signature, const char* mask, size_t sigSize, void* memory, size_t memorySize);

// Signature scan in current process
extern void* sigScan(const char* signature, const char* mask, void* hint = nullptr);