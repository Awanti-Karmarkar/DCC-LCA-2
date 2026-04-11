# 🖧 Network File System (NFS) — Lab Setup Guide

> **Task 2:** Configure NFS server and client · Mount shared directory · Verify access

---

## 📋 Overview

**Network File System (NFS)** is a distributed file system protocol that allows a system to share directories and files with others over a network. In this lab, two virtual machines are configured:

| Role   | IP Address       |
|--------|-----------------|
| Server | `192.168.56.101` |
| Client | `192.168.56.102` |

---

## 🖥️ Part 1 — NFS Server Configuration

> Run all commands in this section on the **Server machine** (`192.168.56.101`)

### Step 1 — Update Package List

```bash
sudo apt update
```

Refreshes the local package index to ensure the latest versions are available before installing anything.

---

### Step 2 — Install NFS Server Package

```bash
sudo apt install nfs-kernel-server -y
```

Installs the NFS kernel server package, which is the core software needed to host NFS shares on Linux.

---

### Step 3 — Create the Shared Directory

```bash
sudo mkdir -p /nfs_shared
```

Creates the directory `/nfs_shared` that will be exported and shared with NFS clients. The `-p` flag prevents errors if the directory already exists.

---

### Step 4 — Set Ownership

```bash
sudo chown nobody:nogroup /nfs_shared
```

Sets the directory owner to `nobody:nogroup` — a generic, unprivileged user/group. This is standard practice for NFS shared directories to avoid permission conflicts across different machines.

---

### Step 5 — Set Permissions

```bash
sudo chmod 777 /nfs_shared
```

Grants full read, write, and execute permissions to all users. This ensures the client machine can freely read and write to the shared directory.

---

### Step 6 — Configure NFS Exports

```bash
sudo nano /etc/exports
```

Opens the NFS exports configuration file for editing. Add the following line:

```
/nfs_shared 192.168.56.102(rw,sync,no_subtree_check)
```

**Export options explained:**

| Option              | Meaning                                                                 |
|---------------------|-------------------------------------------------------------------------|
| `/nfs_shared`       | The directory being shared                                              |
| `192.168.56.102`    | Only this specific client IP is allowed to mount the share             |
| `rw`                | Client has both **read and write** access                              |
| `sync`              | Data is written to disk **before** the server replies (safe/reliable)  |
| `no_subtree_check`  | Disables subtree checking to improve reliability and reduce errors      |

---

### Step 7 — Apply Export Configuration

```bash
sudo exportfs -a
```

Reads `/etc/exports` and exports all defined shares, making them available to clients immediately without a full service restart.

---

### Step 8 — Restart NFS Service

```bash
sudo systemctl restart nfs-kernel-server
```

Restarts the NFS kernel server to apply all configuration changes cleanly.

---

### Step 9 — Verify Exported Shares

```bash
sudo exportfs -v
```

Displays all currently active NFS exports in verbose mode, confirming the share is correctly configured and live.

**Expected output:**
```
/nfs_shared     192.168.56.102(sync,wdelay,hide,no_subtree_check,sec=sys,rw,secure,root_squash,no_all_squash)
```

---

## 💻 Part 2 — NFS Client Configuration

> Run all commands in this section on the **Client machine** (`192.168.56.102`)

### Step 1 — Update Package List

```bash
sudo apt update
```

Refreshes the package index on the client machine.

---

### Step 2 — Install NFS Client Package

```bash
sudo apt install nfs-common -y
```

Installs the NFS common utilities needed for a Linux system to mount and interact with NFS shares.

---

### Step 3 — Create Mount Point

```bash
sudo mkdir -p /mnt/nfs_client
```

Creates a local directory `/mnt/nfs_client` which will act as the mount point — the location where the remote NFS share will appear on the client's file system.

---

### Step 4 — Mount the NFS Share

```bash
sudo mount 192.168.56.101:/nfs_shared /mnt/nfs_client
```

Mounts the NFS share exported by the server (`192.168.56.101`) at the local mount point `/mnt/nfs_client`. After this command, any files created here will be stored on the server.

---

### Step 5 — Verify the Mount

```bash
df -h
```

Displays all mounted file systems in a human-readable format. Confirms the NFS share is mounted and shows its total size and available space.

**Expected output (relevant line):**
```
192.168.56.101:/nfs_shared   ...   /mnt/nfs_client
```

---

## ✅ Verification Checklist

- [ ] NFS server package installed on server
- [ ] `/nfs_shared` directory created with correct permissions
- [ ] `/etc/exports` file updated with correct client IP and options
- [ ] `exportfs -v` shows the share as active
- [ ] NFS client package installed on client
- [ ] Mount point `/mnt/nfs_client` exists on client
- [ ] `sudo mount` command completes without error
- [ ] `df -h` confirms the NFS share is mounted

---

## 🔁 Architecture Diagram

```
┌─────────────────────────────┐          ┌─────────────────────────────┐
│         NFS SERVER          │          │         NFS CLIENT          │
│     192.168.56.101          │          │     192.168.56.102          │
│                             │          │                             │
│  /nfs_shared  ◄─────────────┼──────────┼──► /mnt/nfs_client         │
│  (exported)   NFS Protocol  │          │    (mounted)                │
│               Port 2049     │          │                             │
└─────────────────────────────┘          └─────────────────────────────┘
```

---

## 📝 Notes

- To make the mount **persistent across reboots**, add to `/etc/fstab` on the client:
  ```
  192.168.56.101:/nfs_shared  /mnt/nfs_client  nfs  defaults  0  0
  ```
- To **unmount** the NFS share: `sudo umount /mnt/nfs_client`
- NFS uses **port 2049** by default. Ensure firewall rules allow this port if applicable.
