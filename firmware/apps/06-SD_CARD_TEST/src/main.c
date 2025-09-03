#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/storage/disk_access.h>
#include <zephyr/fs/fs.h>
#include <zephyr/fs/littlefs.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>

LOG_MODULE_REGISTER(main);

#define DISK_DRIVE_NAME "SD"
#define DISK_MOUNT_PT "/sd"

FS_LITTLEFS_DECLARE_DEFAULT_CONFIG(storage);
static struct fs_mount_t lfs_storage_mnt = {
    .type = FS_LITTLEFS,
    .fs_data = &storage,
    .storage_dev = (void *)DISK_DRIVE_NAME,
    .mnt_point = DISK_MOUNT_PT,
};

int main(void)
{
    struct fs_file_t file;
    int ret;
    const char *disk_pdrv = DISK_DRIVE_NAME;
    uint64_t memory_size_mb;
    uint32_t block_count;
    uint32_t block_size;

    // Initialize disk
    ret = disk_access_init(disk_pdrv);
    if (ret != 0) {
        LOG_ERR("Storage init ERROR!");
        return -1;
    }

    // Get disk info
    ret = disk_access_ioctl(disk_pdrv, DISK_IOCTL_GET_SECTOR_COUNT, &block_count);
    if (ret != 0) {
        LOG_ERR("Unable to get sector count");
        return -1;
    }
    
    ret = disk_access_ioctl(disk_pdrv, DISK_IOCTL_GET_SECTOR_SIZE, &block_size);
    if (ret != 0) {
        LOG_ERR("Unable to get sector size");
        return -1;
    }

    memory_size_mb = (uint64_t)block_count * block_size;
    LOG_INF("Block count %u, block size %u, memory size %llu MB",
            block_count, block_size, memory_size_mb >> 20);

    // Mount filesystem
    ret = fs_mount(&lfs_storage_mnt);
    if (ret < 0) {
        LOG_ERR("Error mounting littlefs [%d]", ret);
        return -1;
    }
    LOG_INF("littleFS mounted successfully!");

    // Write data to file
    fs_file_t_init(&file);
    ret = fs_open(&file, DISK_MOUNT_PT "/test.txt", FS_O_CREATE | FS_O_WRITE);
    if (ret < 0) {
        LOG_ERR("Failed to open file [%d]", ret);
        return -1;
    }

    const char *data = "Hello, SD card with littleFS!\n";
    ret = fs_write(&file, data, strlen(data));
    if (ret < 0) {
        LOG_ERR("Failed to write file [%d]", ret);
        fs_close(&file);
        return -1;
    }

    fs_close(&file);
    LOG_INF("Data written successfully!");

    // Read back the data
    ret = fs_open(&file, DISK_MOUNT_PT "/test.txt", FS_O_READ);
    if (ret < 0) {
        LOG_ERR("Failed to open file for reading [%d]", ret);
        return -1;
    }

    char read_buffer[100];
    ret = fs_read(&file, read_buffer, sizeof(read_buffer) - 1);
    if (ret < 0) {
        LOG_ERR("Failed to read file [%d]", ret);
    } else {
        read_buffer[ret] = '\0';
        LOG_INF("Read from file: %s", read_buffer);
    }

    fs_close(&file);

    return 0;
}