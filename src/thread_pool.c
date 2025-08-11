/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_pool.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmondon <jmondon@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 14:10:00 by jmondon           #+#    #+#             */
/*   Updated: 2025/08/10 14:25:00 by jmondon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

typedef struct s_worker_arg { t_fractol_data *d; int id; } t_worker_arg;

/* Store worker args pointer inside the main struct via an unused field (help_text_imgs last slot) is hacky.
   Instead extend struct? Simpler: keep static pointer here. */
static t_worker_arg *g_worker_args = NULL;

static void *worker_real(void *p)
{
    t_worker_arg *wa = (t_worker_arg *)p;
    t_fractol_data *d = wa->d;
    int id = wa->id;
    int local_frame = 0;
    while (true)
    {
        pthread_mutex_lock(&d->mtx);
        while (!d->shutdown && local_frame == d->frame_id)
            pthread_cond_wait(&d->cond, &d->mtx);
        if (d->shutdown)
        { pthread_mutex_unlock(&d->mtx); break; }
        local_frame = d->frame_id;
        int start_y = d->jobs[id].start_y;
        int end_y = d->jobs[id].end_y;
        pthread_mutex_unlock(&d->mtx);
        draw_range(d, start_y, end_y);
        pthread_mutex_lock(&d->mtx);
        d->remaining--;
        if (d->remaining == 0)
            pthread_cond_broadcast(&d->cond);
        pthread_mutex_unlock(&d->mtx);
    }
    return NULL;
}

static void free_jobs_threads(t_fractol_data *d)
{
    if (d->threads)
    {
        for (int i = 0; i < d->thread_count; ++i)
            pthread_join(d->threads[i], NULL);
        free(d->threads);
        d->threads = NULL;
    }
    if (d->jobs) { free(d->jobs); d->jobs = NULL; }
    if (g_worker_args) { free(g_worker_args); g_worker_args = NULL; }
}

void thread_pool_init(t_fractol_data *d, int threads)
{
    if (threads < 1) threads = 1;
    if (threads > MAX_THREADS) threads = MAX_THREADS;
    d->thread_count = threads;
    d->threads = NULL; d->jobs = NULL;
    d->frame_id = 0; d->remaining = 0; d->shutdown = false;
    pthread_mutex_init(&d->mtx, NULL);
    pthread_cond_init(&d->cond, NULL);
    d->threads = malloc(sizeof(pthread_t) * d->thread_count);
    d->jobs = malloc(sizeof(t_job) * d->thread_count);
    g_worker_args = malloc(sizeof(t_worker_arg) * d->thread_count);
    if (!d->threads || !d->jobs || !g_worker_args)
    { ft_printf("Thread allocation failed\n"); exit(EXIT_FAILURE); }
    for (int i = 0; i < d->thread_count; ++i)
    { g_worker_args[i].d = d; g_worker_args[i].id = i; pthread_create(&d->threads[i], NULL, worker_real, &g_worker_args[i]); }
}

void thread_pool_destroy(t_fractol_data *d)
{
    if (!d->threads) return ;
    pthread_mutex_lock(&d->mtx);
    d->shutdown = true;
    pthread_cond_broadcast(&d->cond);
    pthread_mutex_unlock(&d->mtx);
    free_jobs_threads(d);
    pthread_mutex_destroy(&d->mtx);
    pthread_cond_destroy(&d->cond);
}

void thread_pool_resize(t_fractol_data *d, int new_count)
{
    if (new_count < 1) new_count = 1;
    if (new_count > MAX_THREADS) new_count = MAX_THREADS;
    if (new_count == d->thread_count) return ;
    pthread_mutex_lock(&d->mtx);
    d->shutdown = true;
    pthread_cond_broadcast(&d->cond);
    pthread_mutex_unlock(&d->mtx);
    free_jobs_threads(d);
    d->shutdown = false;
    d->thread_count = new_count;
    d->threads = malloc(sizeof(pthread_t) * d->thread_count);
    d->jobs = malloc(sizeof(t_job) * d->thread_count);
    g_worker_args = malloc(sizeof(t_worker_arg) * d->thread_count);
    if (!d->threads || !d->jobs || !g_worker_args)
    { ft_printf("Thread allocation failed (resize)\n"); exit(EXIT_FAILURE); }
    for (int i = 0; i < d->thread_count; ++i)
    { g_worker_args[i].d = d; g_worker_args[i].id = i; pthread_create(&d->threads[i], NULL, worker_real, &g_worker_args[i]); }
}
