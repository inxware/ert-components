# eRT Components licensing

This repository uses a dual‑licensing model that separates the open‑source SDK from a proprietary runtime.

## Summary

* **eRT Components library and build environment**: licensed under **LGPLv3**. See [licenses/eRT_Components.md](licenses/eRT_Components.md).
* **EHS Kernel (binary runtime)**: **proprietary** with its own licence. See [licenses/EHS_Kernel.md](licenses/EHs_Kernel.md).
* When you build an eRT Components project, the build will **pull in the appropriate EHS Kernel** for your target platform.

## Licences by component

### eRT Components (LGPLv3)

You may:

* Use the library in open‑source or closed‑source applications.
* Modify the eRT Components source code.
* Distribute applications that use the library, including commercially.

Your obligations under LGPLv3 include:

* If you modify eRT Components itself, you must make those modifications available under LGPLv3.
* Preserve copyright and licence notices.
* Allow users to relink or replace the LGPL‑licensed library in your product.

Your application code that merely links against eRT Components does **not** need to be released under an open‑source licence.

### EHS Kernel (proprietary)

The EHS Kernel is a closed‑source runtime distributed as a binary. It has a separate end user or commercial licence that governs deployment.

## EHS Kernel deployment terms

* **Free tier**: You may deploy the EHS Kernel on **up to 10 devices** simultaneously at no charge.
* **Commercial licence**: For deployments of **11 to 100 devices**, a paid commercial licence is available.
* **Enterprise licence**: For deployments **above 100 devices**, an enterprise licence is available.

A "device" means a single hardware or virtual instance running the EHS Kernel. The simultaneous device count is measured by active deployments.

## Using eRT Components in your product

1. Develop and build your application with eRT Components (LGPLv3).
2. The build will include the correct EHS Kernel binary for your target.
3. Choose an EHS Kernel licence that matches your deployment size:

   * Up to 10 devices: use the free EHS Kernel licence.
   * 11–100 devices: obtain a commercial licence.
   * More than 100 devices: obtain an enterprise licence.

## Third‑party software

This project may include or link to third‑party libraries. Those components remain under their respective licences.

## Attribution

*Copyright © 2008–2025 inx Limited. The community **eRT Components** release is open‑source under **LGPLv3**. The **EHS Kernel** is proprietary and licensed separately as described above.*
